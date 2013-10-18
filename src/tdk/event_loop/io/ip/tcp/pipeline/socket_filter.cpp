/*
 * socket_filter.cpp
 *
 *  Created on: 2013. 10. 18.
 *      Author: tk
 */

#include <event_loop/io/ip/tcp/pipeline/socket_filter.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

static void _handle_event( tdk::task* t ) {
	socket_filter* filter = static_cast< socket_filter* >( t->context());
	filter->handle_event( static_cast< tdk::io::epoll::task*>(t)->evt() );
}

socket_filter::socket_filter()
	: _epoll_task( &_handle_event , this )
	, _fd(-1)
{

}

socket_filter::~socket_filter() {
	// TODO Auto-generated destructor stub
}

void socket_filter::handle_event( int evt ) {

}

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
