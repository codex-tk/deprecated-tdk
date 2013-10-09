/*
 * connect_task.hpp
 *
 *  Created on: 2013. 10. 8.
 *      Author: tk
 */

#ifndef CONNECT_TASK_HPP_
#define CONNECT_TASK_HPP_

#include <tdk/io/ip/address.hpp>
#include <tdk/event_loop/io_task.hpp>
#include <vector>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class connect_task : public tdk::io_task {
public:
	connect_task();
	connect_task( tdk::task::handler h , void* ctx );
	~connect_task();

	void bind( std::vector< tdk::io::ip::address>& addr );
private:
	std::vector< tdk::io::ip::address> _addrs;
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* CONNECT_TASK_HPP_ */
