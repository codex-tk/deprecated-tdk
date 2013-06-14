#ifndef __tdk_io_engine_h__
#define __tdk_io_engine_h__

#include <tdk/io/completion_port_win32.hpp>
#include <tdk/io/operation.hpp>
#include <tdk/threading/spin_lock.hpp>
#include <tdk/util/list_node.hpp>
#include <tdk/io/engine_detail.hpp>

namespace tdk {
namespace io {

class engine {
public:
	engine( void );
	~engine( void );

	bool open( void );
	void close( void );

	bool run( const tdk::time_span& wait );
	 
	void post( tdk::io::operation* op , const tdk::error_code& ec );
	void async_connect( tdk::io::ip::tcp::connect_operation* op );
	void async_send( tdk::io::ip::tcp::send_operation* op );
	void async_recv( tdk::io::ip::tcp::recv_operation* op );
	void async_accept( tdk::io::ip::tcp::accept_operation* op );

	bool bind( SOCKET fd );

	void req_drain_post_fails( detail::drain_operation* op );
	void do_drain_post_fails( void );

private:
	void _set_drain_handling( detail::drain_operation* op );
private:
	
private:
	completion_port _port;
	// for post fails
	tdk::threading::spin_lock _lock;
	tdk::slist_queue< tdk::io::operation > _op_queue;

	int _drain_in_progress;
};

}}

#endif