/*
 * channel_task.cpp
 *
 *  Created on: 2013. 10. 10.
 *      Author: tk
 */

#include <tdk/event_loop/io/ip/tcp/channel_task.hpp>
#include <tdk/event_loop/io/ip/tcp/channel.hpp>
#include <tdk/event_loop/event_loop.hpp>
namespace tdk {
namespace io {
namespace ip {
namespace tcp {

void execute_impl( tdk::io::ip::tcp::channel* chan , tdk::task* t) {
	chan->loop().execute(t);
}

/*
static void on_channel_thread_task( tdk::task* t ) {
	channel_thread_task* ctt = static_cast< channel_thread_task* >(t);
	ctt->forward();
}

channel_thread_task::channel_thread_task( channel_task* chan_task )
	: tdk::task( &on_channel_thread_task , chan_task)
	, _handler( nullptr )
{

}

channel_thread_task::~channel_thread_task( void ) {

}

void channel_thread_task::set_channel_handler( handler h ) {
	_handler = h;
}

void channel_thread_task::forward(void) {
	channel_task* ct = static_cast< channel_task* >(context());
	if ( _handler ) {
		//channel& chan = *(ct->channel());
		((*(ct->channel())).*_handler)( ct );
	}
}

channel_task::channel_task()
	: _thread_task(this)
	,_channel(nullptr){
	// TODO Auto-generated constructor stub

}

channel_task::channel_task( tdk::task::handler h , void* ctx )
	: io_task( h , ctx )
	, _thread_task(this)
	, _channel( nullptr )
{

}

channel_task::~channel_task() {
	// TODO Auto-generated destructor stub
}

channel_thread_task* channel_task::thread_task( void ) {
	return &_thread_task;
}

tdk::io::ip::tcp::channel* channel_task::channel( void ) {
	return _channel;
}

void channel_task::channel( tdk::io::ip::tcp::channel* chan ) {
	_channel = chan;
}
*/

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
