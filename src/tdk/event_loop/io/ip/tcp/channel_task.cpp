/*
 * channel_task.cpp
 *
 *  Created on: 2013. 10. 10.
 *      Author: tk
 */

#include <tdk/event_loop/io/ip/tcp/channel_task.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

channel_task::channel_task()
	: _channel(nullptr){
	// TODO Auto-generated constructor stub

}

channel_task::channel_task( tdk::task::handler h , void* ctx )
	: io_task( h , ctx )
	, _channel( nullptr )
{

}

channel_task::~channel_task() {
	// TODO Auto-generated destructor stub
}

tdk::task* channel_task::thread_task( void ) {
	return &_thread_task;
}

tdk::io::ip::tcp::channel* channel_task::channel( void ) {
	return _channel;
}

void channel_task::channel( tdk::io::ip::tcp::channel* chan ) {
	_channel = chan;
}


} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
