/*
 * channel.hpp
 *
 *  Created on: 2013. 10. 23.
 *      Author: tk
 */
#if defined(_WIN32)
#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_

#include <tdk/io/ip/socket.hpp>
#include <tdk/io/task.hpp>
#include <tdk/buffer/memory_block.hpp>
#include <tdk/io/ip/udp/pipeline/pipeline.hpp>
#include <atomic>
#include <list>

namespace tdk {
class event_loop;
namespace io {
namespace ip {
namespace udp {

class channel {
public:
	channel( tdk::event_loop& loop , int fd );
	~channel();
	tdk::event_loop& loop( void );

	void close( void );
	void write( const tdk::io::ip::address& addr
		, tdk::buffer::memory_block& msg );

	void error_propagation( const std::error_code& err );

	void do_write( const tdk::io::ip::address& addr
		, tdk::buffer::memory_block& msg );

	udp::pipeline& pipeline( void );

	tdk::io::ip::socket& socket_impl( void );

	// must bo called in filter::on_read
	void pending_read( void );
	void continue_read( void );

	bool is_bits_on( int b );
public:
	void fire_on_open( void );
	void fire_on_read( const tdk::io::ip::address& addr , tdk::buffer::memory_block& msg );
	void fire_on_error( const std::error_code& ec );
	void fire_on_close( void );
	void fire_do_write( const tdk::io::ip::address& addr , tdk::buffer::memory_block msg );
private:
	void _do_recv( void );

	void _error_propagation(const std::error_code& err );
	void _send_remains( void );
	static void _handle_recv( tdk::task* t );
	void handle_recv( void );
	void handle_send( void );

	static void _handle_close( tdk::task* t );
	static void _handle_error_proagation( tdk::task* t );
public:
	void retain( void );
	void release( void );
private:
	tdk::event_loop& _loop;
	tdk::io::ip::socket _socket;
	tdk::io::task _on_recv;
	tdk::task _do_close;
	tdk::io::task _do_error_proagation;
	std::atomic<int> _ref_count;
	std::atomic<int> _state;
	udp::pipeline _pipeline;
	tdk::buffer::memory_block _recv_buffer;
	tdk::io::ip::address _recv_addr;
};

}}}} 

#endif /* CHANNEL_HPP_ */
#endif
