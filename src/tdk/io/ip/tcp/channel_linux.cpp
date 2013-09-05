#include <tdk/io/ip/tcp/channel_linux.hpp>
#include <sys/epoll.h>
#include <tdk/error/error_tdk.hpp>
#include <tdk/error/error_platform.hpp>
#include <tdk/threading/thread_local.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

static void on_connect( tdk::req_handle* req ) {
    epoll::event_handler* handler = static_cast< epoll::event_handler* >( req );
    connect_req* creq = static_cast< connect_req* >( handler->data() );
    creq->channel().handle_connect( creq , handler->events()); 
}

static void on_close( tdk::req_handle* req_h ) {
    close_req* req = static_cast< close_req* >( req_h->data());
    req->channel().handle_close( req );
}

static void on_io_event( tdk::req_handle* handle ) {
    epoll::event_handler* handler = static_cast< epoll::event_handler* >( handle );
    channel* chan = static_cast< channel* >( handler->data());
    chan->handle_io_event( handler->events());
}

static void on_thread_write( tdk::req_handle* req ) {
    write_req* wreq = static_cast< write_req* >( req->data());
    wreq->channel().do_write( wreq );
}

static void on_thread_read( tdk::req_handle* req ) {
    read_req* rreq = static_cast< read_req* >( req->data());
    rreq->channel().do_read( rreq );
}

static void on_reg( tdk::req_handle* req ){
    channel* chan = static_cast< channel* >( req->data());
    chan->handle_reg();
}

static tdk::threading::thread_local<channel*>& current_channel() {   
    static tdk::threading::thread_local< channel* > chan;
    return chan;
}

channel::channel( tdk::loop& loop )
    : _loop( &loop )
{
    _io_handler.set( &on_io_event , this );
    _reg_req.set( nullptr , nullptr );
}

channel::~channel( void ) {

}

tdk::loop& channel::loop( void ) {
    return *_loop;
}

void channel::async_connect( connect_req* req 
        , const tdk::io::ip::address& addr )
{
    std::vector< tdk::io::ip::address > addrs;
    addrs.push_back( addr );
    async_connect( req , addrs );
}

void channel::async_connect( connect_req* req
            , const std::vector< tdk::io::ip::address >& addrs )
{
    req->init( this , addrs );
    do_connect( req );
}

tdk::io::ip::socket& channel::socket_layer( void ){
    return _socket;
}

void channel::do_connect( connect_req* req ) {
    _socket.close();
    tdk::io::ip::address& addr = req->address();
    do {
        if ( _socket.open_tcp( addr.family())) {
            tdk::io::ip::socket::option::non_blocking nb;
            if ( _socket.set_option( nb )){
                bool succ = false;
                do {
                    succ = _socket.connect( addr );
                } while ( ( succ == false ) && ( errno == EINTR ));
                if ( succ || ( errno == EINPROGRESS ) ) {
                    _io_handler.set( &on_connect ,  req );
                    if ( _loop->implementation().reg(
                                _socket.handle()
                                , EPOLLOUT | EPOLLONESHOT 
                                , &_io_handler)){
                        _loop->add_active();
                        return;
                    }
                }
            }
        }
        _socket.close();
    } while ( req->address_next() );
    req->error( tdk::platform::error());
    return _loop->execute( static_cast< tdk::req_handle* >( req ) );
}             

void channel::handle_connect( connect_req* req , int evt ) {
    if ( evt == 0 ) {
        req->error( tdk::tdk_user_abort );
    } else {
        if ( evt & EPOLLERR ){
            req->error( tdk::tdk_epoll_error);
            tdk::io::ip::tcp::channel::socket_layer_type::option::error sock_error;
            if ( socket_layer()
                    .get_option( sock_error ) && sock_error.value()!= 0){
                req->error(tdk::platform::error( sock_error.value()));
            }
        } else if ( evt & EPOLLHUP ) {
            req->error( tdk::tdk_epoll_hang_up );
        } else if ( evt & EPOLLOUT ) {
            req->error( std::error_code());
            tdk::io::ip::tcp::channel::socket_layer_type::option::error sock_error;
            if ( socket_layer()
                    .get_option( sock_error ) && sock_error.value()!= 0){
                req->error(tdk::platform::error( sock_error.value()));
            }
        } else {
            req->error( tdk::tdk_invalid_event );
        }
        if ( req->error() && req->address_next() ) {
            return do_connect( req );
        }
    }
    _io_handler.set( &on_io_event , this );
    _loop->remove_active();
    req->invoke();
}

void channel::async_close( close_req* req ) {
    req->init( this );
    tdk::req_handle* handle = req->channel_close_req();
    handle->set( &on_close , req );
    return _loop->execute( handle );
}

void channel::handle_close( close_req* req ) {
    if ( _socket.handle() != INVALID_SOCKET ) {
        _loop->implementation().unreg( _socket.handle());
    }
    _socket.close();
    _io_handler.events(0);
    _io_handler.invoke();
    req->invoke();
}

void channel::async_read( read_req* req 
            , const tdk::io::buffer_adapter& buffer )
{
    req->init( this , buffer );
    if ( _loop->current_loop() == _loop ) { 
        do_read( req ); 
    } else {
        req->cross_thread_req()->set( 
                &on_thread_read , req );
        _loop->execute( req->cross_thread_req());
    }
}

void channel::do_read( read_req* req ) {
    _loop->add_active();
    _read_reqs.add_tail( req );
    if ( current_channel().get() == this ) 
        return;
    do_reg_req();
}

void channel::async_write( write_req* req
            , const tdk::io::buffer_adapter& buffer ) 
{
    req->init( this , buffer );
    if ( _loop->current_loop() == _loop ) { 
        do_write( req ); 
    } else {
        req->cross_thread_req()->set( 
                &on_thread_write , req );
        _loop->execute( req->cross_thread_req());
    }
}

void channel::do_write( write_req* req ) {
    _loop->add_active();
    _write_reqs.add_tail( req );
    if ( current_channel().get() == this )
        return;
    do_reg_req();
}

void channel::handle_error( const std::error_code& err ) {
    if ( _socket.handle() != INVALID_SOCKET ) 
        _loop->implementation().unreg( _socket.handle());
    _socket.close(); 
    while ( !_read_reqs.is_empty()){
        tdk::io::ip::tcp::read_req* req = static_cast< 
                tdk::io::ip::tcp::read_req* >(_read_reqs.front());
        _read_reqs.pop_front();
        req->error( err );
        _complete_queue.add_tail( req );
    }
}

bool channel::reg( void ) {
    int evt = EPOLLONESHOT;
    if (!_read_reqs.is_empty() ) {
        evt |= EPOLLIN;
    }
    if (!_write_reqs.is_empty() ){
        evt |= EPOLLOUT;
    }
    
    if ( evt == EPOLLONESHOT ) 
        return true;

    if ( _loop->implementation().reg(
                                _socket.handle()
                                , evt
                                , &_io_handler)){
        return true;
    }
    return false;
}

void channel::handle_io_event( int evt ) {
    if ( evt == 0 ) {
        handle_error( tdk::tdk_user_abort );
    } else {
        if ( evt & EPOLLERR ){
            std::error_code ec = tdk::tdk_epoll_error;
            tdk::io::ip::tcp::channel::socket_layer_type::option::error sock_error;
            if ( socket_layer()
                    .get_option( sock_error ) && sock_error.value()!= 0){
                ec = tdk::platform::error( sock_error.value());
            }
            handle_error( ec );
        } 
        current_channel().set( this );
        if ( evt & EPOLLHUP ) {
            handle_error( tdk::tdk_epoll_hang_up );
        } 
        if ( evt & EPOLLIN ) {
            if ( !handle_read() ) {
                handle_error( tdk::platform::error());
            }
        } 
        if ( evt & EPOLLOUT ) {
            if ( !handle_write() ) {
                handle_error( tdk::platform::error());
            }
        }
        current_channel().set( nullptr );
        if ( !reg() ) {
            handle_error( tdk::platform::error());
        }
    }
    handle_complete();
}

bool channel::handle_read( void ){
    while ( !_read_reqs.is_empty() ) {
        tdk::io::ip::tcp::read_req* req = static_cast< 
            tdk::io::ip::tcp::read_req* > ( _read_reqs.front() );

        int readsize = 0;
        do {
            readsize = readv( _socket.handle()
                    , req->buffer_adapter().buffers()
                    , req->buffer_adapter().count());
        } while (( readsize < 0)  && ( errno == EINTR ));
        
        if ( readsize < 0 ) {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK ) ) {
                return true;
            }
            return false;
        } else {
            _read_reqs.pop_front();
            req->io_bytes( readsize );
            _complete_queue.add_tail( req );
        }
    }
    return true;
}

bool channel::handle_write( void ) {
    while ( !_write_reqs.is_empty() ) {
        tdk::io::ip::tcp::write_req* req = static_cast< 
            tdk::io::ip::tcp::write_req* > ( _write_reqs.front() );

        int writesize = 0;
        tdk::io::buffer_adapter write_buffer = req->write_buffer();
        do {
            writesize = writev( _socket.handle()
                    , write_buffer.buffers()
                    , write_buffer.count());
        } while (( writesize < 0)  && ( errno == EINTR ));
        
        if ( writesize < 0 ) {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK ) ) {
                return true;
            }
            return false;
        } else {
            int write_before = req->io_bytes();
            int total_write = req->buffer_adapter().size();
            int writed = write_before + writesize;
            req->io_bytes( writed );
            if ( writed == total_write ) {
                _write_reqs.pop_front();
                _complete_queue.add_tail( req );
            }
        }
    }
    return true;
}

void channel::handle_complete( void ) {
    while ( !_complete_queue.is_empty() ) {
        tdk::req_handle* req = _complete_queue.front();
        _complete_queue.pop_front();
        req->invoke();
        _loop->remove_active();
    }
}

void channel::do_reg_req( void ) {
    if ( _reg_req.data() == nullptr ) {
        _reg_req.set( &on_reg , this );
        _loop->implementation().reg_req( &_reg_req );
    }
}

void channel::handle_reg( void ) {
    if ( !reg()) {
        handle_error( tdk::platform::error() );
    }
    _reg_req.set( nullptr , nullptr );
    handle_complete();
}

bool channel::is_closed( void ) {
    return _socket.handle() == INVALID_SOCKET;
}
   
}}}}



