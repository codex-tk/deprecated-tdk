/*
 * connect_task.hpp
 *
 *  Created on: 2013. 10. 8.
 *      Author: tk
 */

#ifndef CONNECT_TASK_HPP_
#define CONNECT_TASK_HPP_

#include <tdk/io/ip/address.hpp>
#include <tdk/event_loop/io/ip/tcp/socket_task.hpp>
#include <vector>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class channel;
class connect_task : public socket_task {
public:
	connect_task();
	connect_task( tdk::task::handler h , void* ctx );
	~connect_task();

	void address( const std::vector< tdk::io::ip::address>& addr );

	bool address_is_eof( void );
	bool address_next( void );
	tdk::io::ip::address& address_value( void );
private:
	std::vector< tdk::io::ip::address> _addrs;
	std::vector< tdk::io::ip::address>::iterator _addrs_it;
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* CONNECT_TASK_HPP_ */
