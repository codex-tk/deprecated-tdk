/*
 * pipeline_acceptor.hpp
 *
 *  Created on: 2013. 10. 21.
 *      Author: tk
 */

#ifndef PIPELINE_ACCEPTOR_HPP_
#define PIPELINE_ACCEPTOR_HPP_

#include <tdk/io/ip/address.hpp>
#include <tdk/io/ip/socket.hpp>
#include <tdk/event_loop/io/task.hpp>
#include <tdk/event_loop/event_loop.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class pipeline_builder;
class pipeline_acceptor {
public:
	pipeline_acceptor( event_loop& l );
	virtual ~pipeline_acceptor();

	virtual bool condition( const tdk::io::ip::address& addr );

	bool open( const tdk::io::ip::address& addr
			, pipeline_builder* builder );
	void close( void );

	void on_accept_handler( void );
	static void _on_accept_handler(tdk::task* t);
private:
	tdk::event_loop& _loop;
	tdk::io::ip::socket _fd;
	pipeline_builder* _builder;
	tdk::io::task _on_accept;
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* PIPELINE_ACCEPTOR_HPP_ */
