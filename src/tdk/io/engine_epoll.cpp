#include <tdk/io/engine_epoll.hpp>
#include <sys/epoll.h>

namespace tdk {
namespace io{

engine::engine( void ) 
    : _epoll_fd( epoll_create( 1 ) )
{

}

engine::~engine( void ) {
    close( _epoll_fd );
}


}}


