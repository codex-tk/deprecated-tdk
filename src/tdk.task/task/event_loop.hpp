#ifndef __tdk_event_loop_h__
#define __tdk_event_loop_h__

#include <tdk.task/task/win32/io_engine.hpp>
#include <tdk.task/network/tcp/acceptor.hpp>
#include <tdk.task/network/tcp/channel.hpp>

namespace tdk {
namespace task {

class event_loop{
public:
	event_loop( void );
	~event_loop( void );
	
	io_engine& engine( void );

	bool run( const tdk::time_span& wait );

public:
	void on_accept_complete( void );
	void on_connect_complete( void );
	void on_recv_complete( void );
	void on_send_complete( void );


	void on_accept_complete( const tdk::error_code& code
		, tdk::network::tcp::acceptor& acceptor
		, tdk::network::tcp::channel& channel );

	void on_connect_complete( tdk::network::tcp::channel& channel );
	void on_recv_complete( const tdk::error_code& code
		, tdk::network::tcp::channel& channel
		, int io_byte );
	void on_send_complete( tdk::network::tcp::channel& channel );
private:
	io_engine _engine;
};

}}

#endif