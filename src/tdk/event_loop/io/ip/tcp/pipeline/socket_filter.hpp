/*
 * socket_filter.hpp
 *
 *  Created on: 2013. 10. 18.
 *      Author: tk
 */

#ifndef SOCKET_FILTER_HPP_
#define SOCKET_FILTER_HPP_

#include <tdk/event_loop/io/ip/tcp/pipeline/filter.hpp>
#include <tdk/io/ip/socket.hpp>
#include <tdk/event_loop/io/task.hpp>
#include <tdk/buffer/memory_block.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class socket_filter: public tdk::io::ip::tcp::filter {
public:
	socket_filter( SOCKET fd );
	virtual ~socket_filter();

	virtual void on_connect( connect_event& evt );
	virtual void on_recv( recv_event& evt );
	virtual void on_error( error_event& evt );
	virtual void on_close( close_event& e );

	virtual void send( const std::vector< message >& msg );

	void do_recv( void );
	void on_recv_complete( void );

	bool closed( void );
private:
	tdk::io::ip::socket _fd;
	tdk::buffer::memory_block _recv_buffer;
	tdk::io::task _on_recv;
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* SOCKET_FILTER_HPP_ */
