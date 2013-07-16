#include <tdk/io/ip/tcp/socket_epoll.hpp>
#include <tdk/io/engine_epoll.hpp>
#include <tdk/error/error_platform.hpp>
#include <sys/epoll.h>
#include <tdk/error/error_tdk.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {
namespace detail{

static void connect_callback( void* obj , int event ){
    tdk::io::ip::tcp::connect_operation* op = static_cast<
        tdk::io::ip::tcp::connect_operation* >( obj );
    op->socket().handle_connect( op , event );
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
    , _send_op_queue( nullptr )
    , _state(0)
{
    _context.object = this;   
    _context.callback = &detail::socket_callback;
    _state = 0;
}

socket::~socket( void ) {

}

tdk::io::engine& socket::engine( void ) {
    return *_engine;
}

void socket::async_connect( tdk::io::ip::tcp::connect_operation* op ) {
    if ( !open_tcp( op->address().family())){
        engine().post( op , tdk::platform::error());
        return;
    }
    tdk::io::ip::socket::option::non_blocking nb;
    if ( !set_option( nb )){
        engine().post( op , tdk::platform::error());
        return;
    }    
    if ( !connect( op->address())){
        if ( errno != EINPROGRESS ) {
            engine().post( op , tdk::platform::error( errno ));    
            return;
        }
    }
    op->context()->object = op;
    op->context()->callback = &detail::connect_callback;
    if ( !engine().ctl(
              EPOLL_CTL_ADD 
            , op->socket().handle() 
            , EPOLLOUT | EPOLLET | EPOLLONESHOT 
            , static_cast<void*>(op->context())))
    {
        engine().post( op , tdk::epoll_error( errno ));
    }
}

void socket::handle_connect(
        tdk::io::ip::tcp::connect_operation* op 
        , int evt )
{
    std::error_code ec;
    if ( evt & EPOLLERR ) {
        ec = tdk::tdk_epoll_error;
        tdk::io::ip::tcp::socket::option::error sock_error;
        if ( this->get_option( sock_error ) && sock_error.value() != 0 ) {
            ec = tdk::platform::error( sock_error.value());
        }
    } else if ( evt & EPOLLHUP ) {
        ec = tdk::tdk_epoll_hang_up; 
    } else if ( evt & EPOLLOUT ) {
        tdk::io::ip::tcp::socket::option::error sock_error;
        if ( this->get_option( sock_error ) ) {
            ec = tdk::platform::error( sock_error.value());
        } else {
            ec = tdk::platform::error();
        }
    } else {
        ec = tdk::tdk_epoll_error;
    }
    if ( ec ) {
        _state |= detail::SOCKET_ERROR_BIT;
    }
    (*op)( ec , 0 );
}

bool socket::is_error_state( void ) {
    return _state & detail::SOCKET_ERROR_BIT;
}

void socket::async_send( tdk::io::ip::tcp::send_operation* op ) {
    tdk::threading::scoped_lock<> guard( _lock );
    if ( is_error_state() ){
        engine().post( op , tdk::tdk_invalid_call );
        return;
    }
    if ( !_send_op_queue.is_empty()){
        _send_op_queue.add_tail( op );
        return;
    }
    int ret = writev( handle() 
            , op->buffer_adapter().buffers()
            , op->buffer_adapter().count());

    if ( ret >= 0 ) {  
        op->io_bytes( ret );
        engine().post( op , std::error_code());
    } else {
        if ( errno != EAGAIN ) {
            engine().post( op , tdk::platform::error());
        } else {
            int evt = EPOLLET | EPOLLONESHOT | EPOLLOUT ;
            if ( _state & detail::SOCKET_READABLE_BIT == 0 ) {
                evt |= EPOLLIN;
            } 
            if ( !engine().ctl( EPOLL_CTL_MOD 
                    , handle()
                    , evt
                    , &_context ))
            {
                engine().post( op , tdk::epoll_error(errno));
                return;
            }
            _send_op_queue.add_tail( op );
        }
    }
}

void socket::async_recv( tdk::io::ip::tcp::recv_operation* op ) {
    tdk::threading::scoped_lock<> guard( _lock );
    if ( is_error_state()){
        engine().post( op , tdk::tdk_invalid_call );
        return;
    }

    if ( _state & detail::SOCKET_READABLE_BIT ) {
        int ret = readv( handle()
                , op->buffer_adapter().buffers()
                , op->buffer_adapter().count() );

        if ( ret >= 0 ) {
            op->io_bytes( ret );
            engine().post( op , std::error_code());
            return;
        } else {
            if ( errno != EAGAIN ) {
                engine().post( op , tdk::platform::error(errno));
                return;
            }
        }
    } 
    _state &= ~detail::SOCKET_READABLE_BIT;
    int evt = EPOLLET | EPOLLONESHOT | EPOLLIN;
    if ( !_send_op_queue.is_empty()){
        evt |= EPOLLOUT;
    }
    if ( !engine().ctl( EPOLL_CTL_MOD 
                    , handle()
                    , evt
                    , &_context ))
    {
        engine().post( op , tdk::epoll_error(errno));
        return;
    }
    _recv_op_queue.add_tail( op );
}

void socket::handle_send( socket::op_queue& drain  ){
    while( !_send_op_queue.is_empty()){
        tdk::io::ip::tcp::send_operation* send_op = 
                    static_cast< tdk::io::ip::tcp::send_operation*>( _send_op_queue.front());
        int ret = writev( handle()
                , send_op->buffer_adapter().buffers()
                , send_op->buffer_adapter().count());
        if ( ret >= 0 ){
            send_op->io_bytes( ret );
            _send_op_queue.pop_front();
            drain.add_tail( send_op );
        } else {
            if ( errno == EAGAIN ) {
                return ;
            }
            _state |= detail::SOCKET_ERROR_BIT;
            handle_error( drain ,  tdk::platform::error( errno ));
            return ;
        }
    }
}

void socket::handle_recv( socket::op_queue& drain ) {
    if ( _recv_op_queue.is_empty() ) {
        _state |= detail::SOCKET_READABLE_BIT;             
        return ;
    } else {
        while( !_recv_op_queue.is_empty() ) {
            tdk::io::ip::tcp::recv_operation* recv_op
                = static_cast< tdk::io::ip::tcp::recv_operation* >( _recv_op_queue.front());
            int ret = readv( handle()
                    , recv_op->buffer_adapter().buffers()
                    , recv_op->buffer_adapter().count());
            if ( ret >= 0 ) {
                recv_op->io_bytes(ret);
                _recv_op_queue.pop_front();
                drain.add_tail( recv_op );
            } else {
                if ( errno == EAGAIN ){
                    return;
                }
                _state |= detail::SOCKET_ERROR_BIT;
                handle_error( drain , tdk::platform::error(errno));
                return;
            }
        }
                
    }
    return ;
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
}

void socket::handle_epollhup( op_queue& drain ){
    _state |= detail::SOCKET_ERROR_BIT;
    handle_error( drain , tdk::tdk_epoll_hang_up );
}

void socket::handle_epollerr( op_queue& drain ) {
    _state |= detail::SOCKET_ERROR_BIT;
    handle_error( drain , tdk::tdk_epoll_error );
}


void socket::handle_event( int event ) {
    tdk::slist_queue< tdk::io::operation > drains;
    do {
        tdk::threading::scoped_lock<> guard( _lock );
        if ( _state & detail::SOCKET_ERROR_BIT )
            return;
        
        for ( int i = 0 ; i < 4 ; ++i ) {
            if ( _state & detail::SOCKET_ERROR_BIT ) 
                break;
            if ( event & detail::event_mask[i] ) {
                (this->*detail::handler_map[i])(drains);
            }
        }

        if ( _state & detail::SOCKET_ERROR_BIT ) {
            engine().ctl( EPOLL_CTL_DEL
                    , handle()
                    , 0
                    , nullptr );
            break;
        }

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
                _state |= detail::SOCKET_ERROR_BIT;
                handle_error( drains , tdk::epoll_error(errno));
            }
        }

   } while(0);
    
   while( !drains.is_empty() ) {
        auto it = drains.front();
        drains.pop_front();
        (*it)(it->error() , it->io_bytes());
   }

}

}}}}



