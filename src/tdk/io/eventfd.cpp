#include <tdk/io/eventfd.hpp>
#include <tdk/error/error_platform.hpp>

namespace tdk {
namespace io {

eventfd::eventfd( void ){
    _event_fd = ::eventfd(0,EFD_NONBLOCK);
    if ( _event_fd == -1 )
        throw std::system_error( tdk::platform::error());
}

eventfd::~eventfd( void )
{
    close( _event_fd );
}

bool eventfd::set( void ) {
    return eventfd_write( _event_fd , 1 );
}

void eventfd::reset( void ){
    eventfd_t val = 0;
    eventfd_read( _event_fd , &val );
}

int eventfd::handle( void ) {
    return _event_fd;
}

}}
