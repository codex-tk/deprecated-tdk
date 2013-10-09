/*
 * channel.hpp
 *
 *  Created on: 2013. 10. 8.
 *      Author: tk
 */

#ifndef _tdk_io_ip_tcp_channel_h__
#define _tdk_io_ip_tcp_channel_h__

#include <tdk/io/ip/socket.hpp>
#include <tdk/event_loop/event_loop.hpp>

#include <vector>

namespace tdk {
namespace io{
namespace ip{
namespace tcp {

class channel {
public:
	channel( tdk::event_loop& loop );
	~channel();

	void connect( std::vector<tdk::io::ip::address>& addrs );


	void handle_io_event( void );
	void handle_connect_event( void );
private:
	tdk::event_loop* _loop;
	tdk::io::epoll::task _channel_task;
	tdk::io::ip::socket _socket;

	tdk::slist_queue< tdk::task > _read_tasks;
	tdk::slist_queue< tdk::task > _write_tasks;
	tdk::slist_queue< tdk::task > _complete_tasks;
};

}}}}

#endif /* CHANNEL_HPP_ */
