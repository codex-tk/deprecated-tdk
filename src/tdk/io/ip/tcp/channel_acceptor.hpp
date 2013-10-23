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
#include <tdk/io/task.hpp>
#include <tdk/event_loop/event_loop.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class pipeline_builder;
class channel_acceptor {
public:
	channel_acceptor( event_loop& l );
	virtual ~channel_acceptor();

	virtual bool condition( const tdk::io::ip::address& addr );
	virtual event_loop& channel_loop( void );

	bool open( const tdk::io::ip::address& addr
			, pipeline_builder* builder );
	void close( void );

	void on_accept_handler( void );
	static void _on_accept_handler(tdk::task* t);
#if defined( _WIN32)
	void do_accept( void );
#endif
private:
	tdk::event_loop& _loop;
	tdk::io::ip::socket _fd;
	pipeline_builder* _builder;
	tdk::io::ip::address _address;
#if defined (linux) || defined( __linux__)
	tdk::io::task _on_accept;
#elif defined( _WIN32)
	class task : public tdk::io::task {
	public:
		task( tdk::task::handler h , void* ctx  );
		~task( void );

		SOCKET accepted_fd( void );
		void accepted_fd( SOCKET fd );

		tdk::io::ip::address* address( void );
	private:
		SOCKET _accepted_fd;
		tdk::io::ip::address _address[2];
	};
	channel_acceptor::task _on_accept;
	
#endif
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* PIPELINE_ACCEPTOR_HPP_ */
