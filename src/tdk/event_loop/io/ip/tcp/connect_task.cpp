/*
 * connect_task.cpp
 *
 *  Created on: 2013. 10. 8.
 *      Author: tk
 */

#include <tdk/event_loop/io/ip/tcp/connect_task.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

connect_task::connect_task() {

}

connect_task::connect_task( tdk::task::handler h , void* ctx )
	: io_task( h , ctx )
{

}

connect_task::~connect_task() {

}

void connect_task::bind( std::vector< tdk::io::ip::address>& addr ) {
	_addrs = addr;
}

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
