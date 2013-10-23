/*
 * close_task.cpp
 *
 *  Created on: 2013. 10. 10.
 *      Author: tk
 */

#include <tdk/io/ip/tcp/socket/close_task.hpp>
#include <tdk/io/ip/tcp/socket/socket.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

void on_close_handler( tdk::task* t ) {
	close_task* ct = static_cast< close_task* >( t->context());
	ct->socket()->close_impl( ct );
}

close_task::close_task(void)
	: _internal_task( &on_close_handler , this ) 	{

}

close_task::close_task( tdk::task::handler h , void * ctx )
	: socket_task( h , ctx )
	, _internal_task( &on_close_handler , this )
{

}

close_task::~close_task() {
}

tdk::task* close_task::internal_task( void ) {
	return &_internal_task;
}

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
