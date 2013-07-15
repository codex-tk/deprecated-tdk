#include <tdk/io/ip/tcp/operation/connect_operation_epoll.hpp>
#include <tdk/io/ip/tcp/socket_epoll.hpp>
#include <tdk/error/error_platform.hpp>
#include <tdk/error/error_tdk.hpp>
#include <sys/epoll.h>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {
namespace detail {

static void connect_callback( void* obj , int evt ) {
    tcp::connect_operation* op = static_cast< tcp::connect_operation* >( obj );
    op->handle_event( evt );
}

}


connect_operation::connect_operation( tdk::io::operation::callback cb 
    , tdk::io::ip::tcp::socket& fd
    , const std::vector< tdk::io::ip::address >& addrs ) 
    : operation( cb ) 
    , _socket( &fd ) 
    , _address( addrs )
{
    for( auto it : _address ) {
        printf( "ADDR %s:%d\r\n" , it.ip_address().c_str() , it.port());
    }
    _current_end_point = _address.begin();
    _context.object = this;
    _context.callback = &detail::connect_callback;
}

connect_operation::~connect_operation( void )
{

}


bool connect_operation::end_operation( void ) {
    if ( error() ) {
        ++_current_end_point;
        if ( _current_end_point == _address.end() ){
            return true;
        }
        if ( socket().handle() == -1 ) {
            return true;
        }
        socket().close();
        process();
        return false;
    }
    return true;
}

void connect_operation::process( void ) {
    _socket->engine().async_connect( this );
}

tdk::io::ip::tcp::socket& connect_operation::socket( void ) {
    return *_socket;
}

const tdk::io::ip::address& connect_operation::address( void ){
    return *_current_end_point;
}


void connect_operation::handle_event( int evt ) {
    std::error_code ec;
    if ( evt & EPOLLERR ) {
        tdk::io::ip::tcp::socket::option::error sock_error;
        if ( socket().get_option( sock_error ) ) {
            ec = tdk::platform::error( sock_error.value());
        } else {
            ec = tdk::platform::error();
        }
        if ( ec.value() == 0 ){
            ec = tdk::tdk_epoll_error;
        }
    } else if ( evt & EPOLLHUP ) {
        ec = tdk::tdk_epoll_hang_up; 
    } else if ( evt & EPOLLOUT) {
        tdk::io::ip::tcp::socket::option::error sock_error;
        if ( socket().get_option( sock_error ) ) {
            ec = tdk::platform::error( sock_error.value());
        } else {
            ec = tdk::platform::error();
        }
    } else {
        ec = tdk::tdk_epoll_error;
    }
    (*this)( ec , 0 );
}

}}}}

