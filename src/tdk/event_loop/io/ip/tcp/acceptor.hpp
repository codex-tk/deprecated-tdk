/*
 * acceptor.hpp
 *
 *  Created on: 2013. 10. 15.
 *      Author: tk
 */

#ifndef ACCEPTOR_HPP_
#define ACCEPTOR_HPP_

#include <tdk/event_loop/event_loop.hpp>
#include <tdk/io/ip/socket.hpp>
#include <tdk/event_loop/io/ip/tcp/accept_task.hpp>
namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class acceptor {
public:
	acceptor( tdk::event_loop& loop );
	~acceptor();

	bool open( const tdk::io::ip::address& addr , tdk::io::ip::tcp::accept_task* on_accept );
	void close( void );

	void handle_event( int evt );
private:
	tdk::io::epoll::memfn_task< acceptor , void > _acceptor_task;
	tdk::event_loop* _loop;
	tdk::io::ip::socket _socket;
	tdk::io::ip::tcp::accept_task* _on_accept;
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* ACCEPTOR_HPP_ */
