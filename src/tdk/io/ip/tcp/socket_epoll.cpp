#include <tdk/io/ip/tcp/socket_epoll.hpp>
#include <sys/epoll.h>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {
namespace detail{

static void socket_callback( void* obj , int event ) {
    tdk::io::ip::tcp::socket* s = static_cast< tdk::io::ip::tcp::socket* >(obj);
    s->handle_event(event);
}

}

socket::socket( tdk::io::engine& e)
    : _engine( &e )
{
    _context.object = this;    
}

socket::~socket( void ) {

}

tdk::io::engine& socket::engine( void ) {
    return *_engine;
}

void socket::handle_event( int event ) {
    if ( event & EPOLLIN ) {
    }
    if ( event & EPOLLOUT ) {
    }
}

}}}}


