/*
 * channel.hpp
 *
 *  Created on: 2013. 10. 22.
 *      Author: tk
 */

#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_

#include <tdk/io/ip/socket.hpp>
#include <tdk/event_loop/io/task.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/config.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline.hpp>
#include <tdk/buffer/memory_block.hpp>
#include <atomic>
#include <list>

namespace tdk {
class event_loop;
namespace io {
namespace ip {
namespace tcp {

class channel {
public:
	typedef tdk::buffer::memory_block message;
	/*
	class message {
	public:
		message( void );
		message( int type , int sz );
		message( int type , const tdk::buffer::memory_block& mb );
		message( const message& m );
		message& operator=( const message& m );

		~message( void );

		int type( void );
		void type( int t );

		tdk::buffer::memory_block& data( void );
		void data( const tdk::buffer::memory_block& mb );
	private:
		int _type;
		tdk::buffer::memory_block _mb;
	};*/
public:
	channel( tdk::event_loop& loop , int fd );
	~channel();
	tdk::event_loop& loop( void );

	void close( void );
	void write( tdk::buffer::memory_block& msg );

	void error_propagation( const std::error_code& err );
	void do_write( tdk::buffer::memory_block& msg );

	tcp::pipeline& pipeline( void );
public:
	void fire_on_connected( void );
	void fire_on_accepted( const tdk::io::ip::address& addr );
	void fire_on_read( tdk::buffer::memory_block& msg );
	void fire_on_write( int writed , bool flushed );
	void fire_on_error( const std::error_code& ec );
	void fire_on_close( void );
	void fire_do_write( tdk::buffer::memory_block msg );
private:
	void _error_propagation(const std::error_code& err );
	void _register_handle(void);
	void _handle_readable( void );
	void _handle_writeable( void );
	bool _send_remains( void );

	static void _handle_io_events( tdk::task* t );
	void handle_io_events( void );
public:
	void retain( void );
	void release( void );
private:
	tdk::event_loop& _loop;
	tdk::io::ip::socket _socket;
	tdk::io::task _io_context;
	std::atomic<int> _state;
	std::list< tdk::buffer::memory_block > _send_queue;
	std::atomic< int > _ref_count;
	tcp::pipeline _pipeline;
public:
	static config& channel_config( void );
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* CHANNEL_HPP_ */
