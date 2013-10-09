/*
 * channel.cpp
 *
 *  Created on: 2013. 10. 8.
 *      Author: tk
 */

#include <tdk/event_loop/io/ip/tcp/channel.hpp>

namespace tdk {
namespace io{
namespace ip{
namespace tcp {

static void channel_connect_handler( tdk::task* t);
static void channel_io_handler( tdk::task* t);



channel::channel( tdk::event_loop& loop )
	: _loop( &loop )
	, _channel_task( &channel_io_handler , this )
{

}

channel::~channel() {

}

void channel::connect(std::vector<tdk::io::ip::address>& addrs ) {

}

void channel::handle_io_event( void ) {

}

void channel::handle_connect_event( void ) {

}


static void channel_connect_handler( tdk::task* t ) {
	channel* ch = static_cast< channel* >( t->context());
	ch->handle_connect_event();
}

static void channel_io_handler( tdk::task* t ) {
	channel* ch = static_cast< channel* >( t->context());
	ch->handle_io_event();
}

}}}}

