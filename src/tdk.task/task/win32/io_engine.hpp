#ifndef __tdk_task_win32_io_engine_h__
#define __tdk_task_win32_io_engine_h__

#include <tdk/error/error_code.hpp>

#include <tdk.task/task/win32/io_completion_port.hpp>
#include <tdk.task/task/operation.hpp>

#include <tdk.task/network/tcp/acceptor.hpp>
#include <tdk.task/network/tcp/connector.hpp>
#include <tdk.task/network/tcp/channel.hpp>
#include <tdk.task/network/udp/channel.hpp>
#include <tdk/threading/spin_lock.hpp>
#include <vector>
#include <atomic>

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

	void add_accept_io( tdk::network::tcp::accept_operation* op );
	void add_recv_io( tdk::network::tcp::recv_operation* op );
	void add_recv_io( tdk::network::tcp::recv_operation* op , int size );
	void add_send_io( tdk::network::tcp::send_operation* op );
	void add_connect_io( tdk::network::tcp::connect_operation* op );
	void add_recvfrom_io( tdk::network::udp::recvfrom_operation* op );

	void on_complete( 
		const std::error_code& code 
		, int io_byte 
		, void* key
		, OVERLAPPED* ov );

	bool run( const tdk::time_span& wait );

	void post( operation* op );

private:
	event_loop& _loop;
	io_completion_port _port;
	std::atomic<int> _post_failed;
	tdk::threading::spin_lock _op_queue_lock;
	tdk::slist_queue< operation > _op_queue;
};

}}

#endif