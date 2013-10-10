/*
 * close_task.cpp
 *
 *  Created on: 2013. 10. 10.
 *      Author: tk
 */

#include <tdk/event_loop/io/ip/tcp/close_task.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

close_task::close_task() {
	// TODO Auto-generated constructor stub

}

close_task::close_task( tdk::task::handler h , void * ctx )
	: channel_task( h , ctx ){

}

close_task::~close_task() {
	// TODO Auto-generated destructor stub
}

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
