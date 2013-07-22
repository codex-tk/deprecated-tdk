#include <tdk/io/ip/tcp/socket_epoll.hpp>
#include <tdk/io/engine_epoll.hpp>
#include <tdk/error/error_platform.hpp>
#include <sys/epoll.h>
#include <tdk/error/error_tdk.hpp>
#include <tdk/log/logger.hpp>
#include <tdk/log/writer/stderr_writer.hpp>

#ifndef SOCKET_LOG
#define SOCKET_LOG( msg , ... ) LOG_D( tdk::io::ip::tcp::detail::socket_logger() , msg , __VA_ARGS__ )
//#define SOCKET_LOG( msg , ... )
#endif

namespace tdk {
namespace io {
namespace ip {
namespace tcp {
namespace detail{

tdk::log::logger& socket_logger(){
    static tdk::log::logger log(tdk::tstring("socket") 
            , tdk::log::stderr_writer::instance());
    return log;
}

static void connect_callback( void* obj , int event ){
    tdk::io::ip::tcp::connect_operation* op = static_cast<
        tdk::io::ip::tcp::connect_operation* >( obj );
    op->socket().handle_connect( op , event );
}

static void accept_callback( void* obj , int event ){
    tdk::io::ip::tcp::socket* s = static_cast< tdk::io::ip::tcp::socket* >(obj);
    s->handle_accept(event);
}

static void socket_callback( void* obj , int event ) {
    tdk::io::ip::tcp::socket* s = static_cast< tdk::io::ip::tcp::socket* >(obj);
    s->handle_event(event);
}

enum socket_bits {
    SOCKET_READABLE_BIT  = 0x01,
    SOCKET_WRITEABLE_BIT = 0x02,    
    SOCKET_ERROR_BIT     = 0x04,
};

typedef void (socket::*handler)( socket::op_queue& );

static handler handler_map[] = {
      &tcp::socket::handle_epollhup
    , &tcp::socket::handle_epollerr
    , &tcp::socket::handle_recv
    , &tcp::socket::handle_send
};

static int event_mask[] = {
      EPOLLHUP
    , EPOLLERR
    , EPOLLIN
    , EPOLLOUT
};

}

socket::socket( tdk::io::engine& e)
    : _engine( &e )
    , _state(0)
{
    _context.object = this;   
    _context.callback = &detail::socket_callback;
    SOCKET_LOG("[%p] ctor" , this );
}

socket::~socket( void ) {
    SOCKET_LOG("[%p] dtor" , this  );
}

tdk::io::engine& socket::engine( void ) {
    return *_engine;
}

void socket::handle_close( tdk::io::operation* op ){
    tdk::slist_queue< tdk::io::operation > drains;
    do {
        tdk::threading::scoped_lock<> gaurd( _lock );
        handle_error( drains , tdk::tdk_network_user_abort );
    }while(0);
    while( !drains.is_empty() ) {
        auto it = drains.front();
        drains.pop_front();
        (*it)(it->error() , it->io_bytes());
        engine().dec_posted();
    }
    if ( op ){
        (*op)( op->error() , 0 );
    }
}

void socket::async_close(tdk::io::operation* op ){
    tdk::threading::scoped_lock<> gaurd( _lock);
    if ( handle() != -1 ) {
        engine().ctl( EPOLL_CTL_DEL 
                    , handle() 
                    , 0
                    , nullptr );
        state_set( detail::SOCKET_ERROR_BIT );
    }
    tdk::io::ip::socket::close();  
    _state = 0;
    engine().post(
            [this,op](void){
                handle_close(op);
            });
}

void socket::async_connect( tdk::io::ip::tcp::connect_operation* op ) {
    std::error_code ec;
    if ( !open_tcp( op->address().family())){
        ec = tdk::platform::error();
        SOCKET_LOG( "[%p] open tcp fail %s" , this , ec.message().c_str());
        engine().post( op , ec );
        return;
    }
    tdk::io::ip::socket::option::non_blocking nb;
    if ( !set_option( nb )){
        ec = tdk::platform::error();
        SOCKET_LOG( "[%p] set_nonblock fail %s" , this , ec.message().c_str());
        engine().post( op , ec );
        return;
    }    
    if ( !connect( op->address())){
        if ( errno != EINPROGRESS ){
            ec = tdk::platform::error();
            SOCKET_LOG( "[%p] connect fail %s" , this , ec.message().c_str());
            engine().post( op , ec);
            return;
        }
    }
    op->context()->object   = op;
    op->context()->callback = &detail::connect_callback;
    if ( !engine().ctl(
              EPOLL_CTL_ADD 
            , op->socket().handle() 
            , EPOLLOUT | EPOLLET | EPOLLONESHOT 
            , static_cast<void*>(op->context())))
    {
        ec = tdk::epoll_error(errno);
        SOCKET_LOG( "[%p] epoll_ctl error %s" , this , ec.message().c_str());
        engine().post( op , ec );
        return;
    }
    printf( "EPOLL ADD BIT %d\r\n" , _state );
    engine().inc_posted();
}

void socket::handle_connect(
        tdk::io::ip::tcp::connect_operation* op 
        , int evt )
{
    std::error_code ec;
    do {
        tdk::threading::scoped_lock<> guard( _lock );
        if ( evt & EPOLLERR ) {
            SOCKET_LOG( "[%p] epoll error" , this );
            ec = tdk::tdk_epoll_error;
            tdk::io::ip::tcp::socket::option::error sock_error;
            if ( this->get_option( sock_error ) && sock_error.value() != 0 ) {
                ec = tdk::platform::error( sock_error.value());
                SOCKET_LOG( "[%p] getsockerror %s" , this , ec.message().c_str());
            }
        } else if ( evt & EPOLLHUP ) {
            ec = tdk::tdk_epoll_hang_up; 
            SOCKET_LOG("[%p] epoll hang up", this );
        } else if ( evt & EPOLLOUT ) {
            tdk::io::ip::tcp::socket::option::error sock_error;
            if ( this->get_option( sock_error ) ) {
                ec = tdk::platform::error( sock_error.value());
                SOCKET_LOG( "[%p] epoll out %s" , this ,  ec.message().c_str());
            } else {
                ec = tdk::platform::error();
                SOCKET_LOG( "[%p] epollout getsockopt fail %s" , this, ec.message().c_str());
            }
        } else {
            SOCKET_LOG( "[%p] unexpected event %d" , this , evt );
            ec = tdk::tdk_epoll_error;
        }
        if ( ec ) {
            state_set( detail::SOCKET_ERROR_BIT );
        }
    } while(0);
    (*op)( ec , 0 );
    engine().dec_posted();
}

bool socket::is_error_state( void ) {
    return _state & detail::SOCKET_ERROR_BIT;
}

bool socket::_write( tdk::io::ip::tcp::send_operation* op ){
    int data_size = op->buffer_adapter().size();
    while ( true ) {
        tdk::io::buffer_adapter buffer = op->write_buffer();
        int ret = writev( handle()
                , buffer.buffers()
                , buffer.count());
        if ( ret >= 0 ) {
            int send_size = op->io_bytes() + ret;
            op->io_bytes( send_size );
            if ( send_size >= data_size ) {
                return true;
            }
        } else {
            if ( errno == EINTR ) {
                continue;
            }
            return false;
        }
    }
}

bool socket::_read( tdk::io::ip::tcp::recv_operation* op ) {
    while(true) {
        int ret = readv( handle()
                , op->buffer_adapter().buffers()
                , op->buffer_adapter().count() );
        if( ret >= 0 ) {
            op->io_bytes( ret );
            return true;
        } else {
            if ( errno == EINTR )
                continue;
            return false;
        }
    }
}

void socket::async_send( tdk::io::ip::tcp::send_operation* op ) {
    tdk::threading::scoped_lock<> guard( _lock );
    if ( is_error_state() ){
        SOCKET_LOG( "[%p] is error state" ,this ); 
        engine().post( op , tdk::tdk_invalid_call );
        return;
    }
    if ( !_send_op_queue.is_empty()){
        SOCKET_LOG( "[%p] send pending" ,this);
        _send_op_queue.add_tail( op );
        engine().inc_posted();
        return;
    }
    
    if ( _write( op )) {  
        SOCKET_LOG( "[%p] writev success" , this );
        engine().post( op , std::error_code());
    } else {
        if ( errno != EAGAIN ) {
            SOCKET_LOG( "[%p] writev error %s" , this , tdk::platform::error().message().c_str());
            engine().post( op , tdk::platform::error());
        } else {
            if ( !engine().ctl( EPOLL_CTL_MOD  
                    , handle()
                    , EPOLLIN | EPOLLET | EPOLLONESHOT | EPOLLOUT
                    , &_context ))
            {
                SOCKET_LOG( "[%p] epoll ctl error %s" , tdk::epoll_error(errno).message().c_str()); 
                engine().post( op , tdk::epoll_error(errno));
                return;
            }
            SOCKET_LOG( "[%p] epoll ctl success" , this );
            _send_op_queue.add_tail( op );
        }
    }
}

void socket::async_recv( tdk::io::ip::tcp::recv_operation* op ) {
    tdk::threading::scoped_lock<> guard( _lock );
    if ( is_error_state()){
        SOCKET_LOG( "[%p] is error state" , this );
        engine().post( op , tdk::tdk_invalid_call );
        return;
    }

    if ( !_recv_op_queue.is_empty() ){
        _recv_op_queue.add_tail( op );
        engine().inc_posted();
        return;
    }

    if ( state_get( detail::SOCKET_READABLE_BIT )) {
        if ( _read( op ) ) {
            SOCKET_LOG( "[%p] readv success %d" , this , op->io_bytes());
            engine().post( op , std::error_code());
            return;
        } else {
            if ( errno != EAGAIN ) {
                SOCKET_LOG( "[%p] readv error %s" , this , tdk::platform::error(errno).message().c_str());
                engine().post( op , tdk::platform::error(errno));
                return;
            }
        }
    } 
    state_clear( detail::SOCKET_READABLE_BIT );
    int evt = EPOLLET | EPOLLONESHOT | EPOLLIN;
    if ( !_send_op_queue.is_empty()){
        evt |= EPOLLOUT;
    }
    if ( !engine().ctl( EPOLL_CTL_MOD 
                    , handle()
                    , evt
                    , &_context ))
    {
        SOCKET_LOG( "[%p] epoll ctl error %s" , this ,  tdk::epoll_error(errno).message().c_str());
        engine().post( op , tdk::epoll_error(errno));
        return;
    }
    SOCKET_LOG( "[%p] epoll ctl success" ,this );
    _recv_op_queue.add_tail( op );
    engine().inc_posted();
}

void socket::handle_send( socket::op_queue& drain  ){
    while( !_send_op_queue.is_empty()){
        tdk::io::ip::tcp::send_operation* send_op = 
                    static_cast< tdk::io::ip::tcp::send_operation*>( _send_op_queue.front());
        
        if ( _write(send_op)){
            SOCKET_LOG( "[%p] writev success " , this  );
            _send_op_queue.pop_front();
            drain.add_tail( send_op );
        } else {
            if ( errno == EAGAIN ) {
                SOCKET_LOG( "[%p] write re request" , this );
                return ;
            }
            state_set( detail::SOCKET_ERROR_BIT );
            SOCKET_LOG( "[%p] error %s" , this , tdk::platform::error(errno).message().c_str());
            handle_error( drain ,  tdk::platform::error( errno ));
            return ;
        }
    }
}

void socket::handle_recv( socket::op_queue& drain ) {
    state_set( detail::SOCKET_READABLE_BIT );             
    if ( _recv_op_queue.is_empty() ) {
        SOCKET_LOG( "[%p] on readable", this ); 
        return ;
    } else {
        while( !_recv_op_queue.is_empty() ) {
            tdk::io::ip::tcp::recv_operation* recv_op
                = static_cast< tdk::io::ip::tcp::recv_operation* >( _recv_op_queue.front());
            if ( _read( recv_op )) {
                SOCKET_LOG( "[%p] readv success %d" , this , recv_op->io_bytes());
                _recv_op_queue.pop_front();
                drain.add_tail( recv_op );
            } else {
                if ( errno == EAGAIN ){
                    state_clear( detail::SOCKET_READABLE_BIT );
                    SOCKET_LOG( "[%p] read re request" , this );
                    return;
                }
                state_set(detail::SOCKET_ERROR_BIT );
                SOCKET_LOG( "[%p] error %s" , this , tdk::platform::error(errno).message().c_str());
                handle_error( drain , tdk::platform::error(errno));
            }
        }
    }
}

bool socket::_open( int fd ) {
    handle( fd );
    tdk::io::ip::tcp::socket::option::non_blocking nb;
    set_option( nb );
    return engine().ctl( EPOLL_CTL_ADD 
                 , handle()
                 , EPOLLET | EPOLLONESHOT
                 , &_context ) == 0;
}

int socket::_accept( tdk::io::ip::address& addr ) {
    int fd = 0;
    while ( true ) {
        fd = ::accept( handle() , addr.sockaddr() , addr.sockaddr_length_ptr());
        if ( fd >= 0 )
            return true;
        else {
            if (errno == EINTR )
                continue;
            return false;
        }
    }
}

void socket::async_accept( tdk::io::ip::tcp::accept_operation* op ){
    tdk::threading::scoped_lock<> guard( _lock );
    if ( is_error_state()){
        SOCKET_LOG( "[%p] is error state" , this );
        engine().post( op , tdk::tdk_invalid_call );
        return;
    }

    if ( !_accept_op_queue.is_empty() ){
        _accept_op_queue.add_tail( op );
        engine().inc_posted();
        return;
    }

    if ( state_get( detail::SOCKET_READABLE_BIT )) {
        tdk::io::ip::address addr;
        int fd = _accept( addr );
        if ( fd >= 0 ) {
            SOCKET_LOG( "[%p] accept success" , this );
            op->socket()._open( fd );
        } else {
            if ( errno != EAGAIN ) {
                SOCKET_LOG( "[%p] accept error %s" , this , tdk::platform::error(errno).message().c_str());
                engine().post( op , tdk::platform::error(errno));
                return;
            }
            SOCKET_LOG( "[%p] socket accept re issue" , this );
        }
    } 
    state_clear( detail::SOCKET_READABLE_BIT );
    int evt = EPOLLET | EPOLLONESHOT | EPOLLIN;
    
    _context.object = this;
    _context.callback = &detail::accept_callback;

    if ( !engine().ctl( EPOLL_CTL_MOD 
                    , handle()
                    , evt
                    , &_context ))
    {
        SOCKET_LOG( "[%p] epoll ctl error %s" , this ,  tdk::epoll_error(errno).message().c_str());
        engine().post( op , tdk::epoll_error(errno));
        return;
    }
    SOCKET_LOG( "[%p] epoll ctl success" ,this );
    _accept_op_queue.add_tail( op );
    engine().inc_posted();
}

void socket::handle_accept( int evt ){
    tdk::threading::scoped_lock<> guard( _lock );
    op_queue ops;
    do {
        if ( evt & EPOLLHUP ) {
            handle_error( ops , tdk::tdk_epoll_hang_up );
            break;
        }
        if ( evt & EPOLLERR ) {
            handle_error( ops , tdk::tdk_epoll_error );
            break;
        }

        if ( evt & EPOLLIN ) {
            state_set( detail::SOCKET_READABLE_BIT );             
            if ( _accept_op_queue.is_empty() ) {
                SOCKET_LOG( "[%p] on acceptable", this ); 
            } else {
                while ( !_accept_op_queue.is_empty()) {
                    tdk::io::ip::tcp::accept_operation* op
                        = static_cast< tdk::io::ip::tcp::accept_operation* >( _accept_op_queue.front());
                    tdk::io::ip::address addr;
                    int fd = _accept( addr );
                    if ( fd  >= 0 ) {
                        SOCKET_LOG( "[%p] accept success " , this );
                        _accept_op_queue.pop_front();
                        op->socket()._open( fd );
                        ops.add_tail( op );
                    } else {
                        if ( errno == EAGAIN ){
                            state_clear( detail::SOCKET_READABLE_BIT );
                            SOCKET_LOG( "[%p] accept re request" , this );
                            int evt = EPOLLET | EPOLLONESHOT | EPOLLIN;
                            if ( !engine().ctl( EPOLL_CTL_MOD 
                                , handle()
                                , evt
                                , &_context ))
                            {
                                SOCKET_LOG( "[%p] epoll ctl error %s" , this ,  tdk::epoll_error(errno).message().c_str());
                                handle_error( ops , tdk::platform::error(errno));
                            }
                        } else {
                            state_set(detail::SOCKET_ERROR_BIT );
                            SOCKET_LOG( "[%p] error %s" , this , tdk::platform::error(errno).message().c_str());
                            handle_error( ops , tdk::platform::error(errno));
                        }
                        break;
                    }                    
                }
            }
        }
    }while(0);
    while( !ops.is_empty() ) {
        auto it = ops.front();
        ops.pop_front();
        (*it)(it->error() , it->io_bytes());
        engine().dec_posted();
    }
}

void socket::handle_error( op_queue& drain , const std::error_code& ec ){
    while ( !_recv_op_queue.is_empty() ) {
        auto op = _recv_op_queue.front();
        _recv_op_queue.pop_front();
        op->error( ec );
        drain.add_tail(op);
    }
    while ( !_send_op_queue.is_empty() ) {
        auto op = _send_op_queue.front();
        _send_op_queue.pop_front();
        op->error( ec );
        drain.add_tail(op);
    }
    while ( !_accept_op_queue.is_empty() ) {
        auto op = _accept_op_queue.front();
        _accept_op_queue.pop_front();
        op->error( ec );
        drain.add_tail(op);
    }
}

void socket::handle_epollhup( op_queue& drain ){
    SOCKET_LOG( "[%p] epoll hup" , this );
    _state |= detail::SOCKET_ERROR_BIT;
    handle_error( drain , tdk::tdk_epoll_hang_up );
}

void socket::handle_epollerr( op_queue& drain ) {
    SOCKET_LOG( "[%p] epoll err" ,  this );
    _state |= detail::SOCKET_ERROR_BIT;
    handle_error( drain , tdk::tdk_epoll_error );
}

bool socket::register_handle_event( void ){
    int evt = 0;
    if ( !_recv_op_queue.is_empty()){
        evt |= EPOLLIN;
    }
    if ( !_send_op_queue.is_empty()){
        evt |= EPOLLOUT;
    }
    if ( evt != 0 ) {
        evt |= EPOLLET | EPOLLONESHOT;
        if ( !engine().ctl( EPOLL_CTL_MOD 
                , handle()
                , evt
                , &_context ))
        {
            state_set( detail::SOCKET_ERROR_BIT );
            SOCKET_LOG( "[%p] epoll ctl fail %d %s" 
                    , this
                    , evt
                    , tdk::epoll_error(errno).message().c_str());
            return false;
        }
    }
    return true;
}

void socket::handle_event( int event ) {
    tdk::slist_queue< tdk::io::operation > drains;
    do {
        tdk::threading::scoped_lock<> guard( _lock );
        if ( is_error_state() )
            return;
        
        for ( int i = 0 ; i < 4 ; ++i ) {
            if ( is_error_state() )
                break;
            if ( event & detail::event_mask[i] ) {
                (this->*detail::handler_map[i])(drains);
            }
        }
        if ( !register_handle_event()){
            handle_error( drains , tdk::epoll_error(errno));
        }
   } while(0);
    
   while( !drains.is_empty() ) {
        auto it = drains.front();
        drains.pop_front();
        (*it)(it->error() , it->io_bytes());
        engine().dec_posted();
   }
}


void socket::state_set( int s ){
    _state |= s;
}

void socket::state_clear( int s ) {
    _state &= ~s;
}

bool socket::state_get( int s ){
    return ( _state & s ) != 0 ;
}

bool socket::open_accept( const tdk::io::ip::address& addr ) {
    if ( !open_tcp( addr.family())) {
        return false;
    }
    
    tdk::io::ip::tcp::socket::option::reuse_address reuse;
    tdk::io::ip::tcp::socket::option::non_blocking nb;
    if ( !set_option( reuse )){
        return false;
    }
    
    if ( !set_option( nb ) ){
        return false;
    }
    if ( !bind( addr )){
        return false;
    }
    if ( !listen()){
        return false;
    }
    
    return engine().ctl( EPOLL_CTL_ADD
            , handle()
            , EPOLLET | EPOLLONESHOT
            , &_context );
    
}


}}}}


