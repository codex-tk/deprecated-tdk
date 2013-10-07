#include "stdafx.h"
#include <tdk/event_loop/io/epoll.hpp>


namespace tdk {
namespace io {

epoll::task::~task( void ) {

}
epoll::task::task( void ) {

}

int epoll::task::evt( void ) {

}

void epoll::task::evt( int e ) {

}

epoll::event_fd_task::event_fd_task( void ) {

}

epoll::event_fd_task::~event_fd_task( void ) {

}

int epoll::event_fd_task::handle( void ) {

}

void epoll::event_fd_task::set( void ) {

}

epoll::epoll( void ) {

}

epoll::~epoll( void ) {

}

bool epoll::register_handle( int fd , epoll::task* task ) {

}

void epoll::unregister_handle( int fd ) {

}

void epoll::wake_up( void ) {

} 

void epoll::wait( const tdk::time_span& w ) {
	:
}

}
}