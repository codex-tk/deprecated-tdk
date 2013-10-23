/*
 * read_task.cpp
 *
 *  Created on: 2013. 10. 10.
 *      Author: tk
 */

#include <tdk/io/ip/tcp/socket/read_task.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

read_task::read_task( void )
{
}

read_task::read_task( tdk::task::handler h , void* ctx )
	: socket_task( h , ctx )
{

}

read_task::~read_task( void ) {

}

void read_task::buffers( const tdk::io::buffer_adapter& buf )
{
	_buffer = buf;
}

tdk::io::buffer_adapter& read_task::buffers( void ) {
	return _buffer;
}


} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
