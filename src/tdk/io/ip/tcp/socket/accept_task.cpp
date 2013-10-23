/*
 * accept_task.cpp
 *
 *  Created on: 2013. 10. 15.
 *      Author: tk
 */

#include <tdk/event_loop/io/ip/tcp/accept_task.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

accept_task::accept_task()
	: _fd(-1)
{
	// TODO Auto-generated constructor stub

}

accept_task::accept_task( tdk::task::handler h , void* ctx )
	: tdk::io::task( h , ctx ) {

}

accept_task::~accept_task() {
	// TODO Auto-generated destructor stub
}

int accept_task::accepted( void ) {
	return _fd;
}

void accept_task::accepted( int fd ) {
	_fd = fd;
}

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
