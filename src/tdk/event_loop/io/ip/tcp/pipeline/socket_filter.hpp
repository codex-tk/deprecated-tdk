/*
 * socket_filter.hpp
 *
 *  Created on: 2013. 10. 18.
 *      Author: tk
 */

#ifndef SOCKET_FILTER_HPP_
#define SOCKET_FILTER_HPP_

#include <tdk/event_loop/io/ip/tcp/pipeline/filter.hpp>
#include <tdk/event_loop/io/epoll.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class socket_filter: public tdk::io::ip::tcp::filter {
public:
	socket_filter();
	virtual ~socket_filter();

	void handle_event( int evt );
private:
	tdk::io::epoll::task _epoll_task;
	int _fd;
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* SOCKET_FILTER_HPP_ */
