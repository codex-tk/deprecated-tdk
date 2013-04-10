#ifndef __tdk_task_win32_io_engine_h__
#define __tdk_task_win32_io_engine_h__

#include <tdk/error/error_code.hpp>

#include <tdk.task/task/win32/io_completion_port.hpp>
#include <tdk.task/task/win32/io_contexts.hpp>

#include <tdk.task/network/tcp/acceptor.hpp>
#include <tdk.task/network/tcp/channel.hpp>

#include <tdk/threading/spin_lock.hpp>
#include <tdk/threading/atomic/atomic.hpp>
#include <vector>

namespace tdk{
namespace task {

class event_loop;
class io_engine {
public:
	
public:
	io_engine( event_loop& loop );
	~io_engine( void );

	bool open( void );
	void close( void );

	bool bind( SOCKET fd , void* obj );

	void add_accept_io( 
		tdk::network::tcp::acceptor& acceptor );

	void add_recv_io( 
		tdk::network::tcp::channel& chan 
		, tdk::buffer::memory_block& mb );

	void on_complete( 
		const tdk::error_code& code 
		, int io_byte 
		, void* key
		, OVERLAPPED* ov );

	bool run( const tdk::time_span& wait );

private:
	void _post( io_context* ctx );

private:
	event_loop& _loop;
	io_completion_port _port;
	tdk::threading::atomic<int> _post_failed;
	tdk::threading::spin_lock _post_fail_lock;
	std::vector< io_context * > _post_fails;
};

}}

#endif