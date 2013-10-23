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

connect_task::connect_task(){

}

connect_task::connect_task( tdk::task::handler h , void* ctx )
	: socket_task( h , ctx )
{

}

connect_task::~connect_task() {

}

void connect_task::address( const  std::vector< tdk::io::ip::address>& addr ) {
	_addrs = addr;
	_addrs_it = _addrs.begin();
}

bool connect_task::address_is_eof( void ) {
	return _addrs_it == _addrs.end();
}

bool connect_task::address_next( void ) {
	if ( address_is_eof())
		return false;
	++_addrs_it;
	return address_is_eof();
}

tdk::io::ip::address& connect_task::address_value( void ) {
	return *_addrs_it;
}


} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
