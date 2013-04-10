#ifndef __tdk_event_loop_h__
#define __tdk_event_loop_h__

#include <tdk.task/task/win32/io_engine.hpp>
#include <tdk.task/network/tcp/acceptor.hpp>
#include <tdk.task/network/tcp/channel.hpp>

namespace tdk {
namespace task {

class event_loop{
public:
	typedef void (__stdcall* on_accept)( const tdk::error_code& code
		, tdk::network::tcp::acceptor& acceptor
		, tdk::network::socket fd 
		, void* ctx );

	typedef void (__stdcall* on_recv)( const tdk::error_code& code
		, tdk::network::tcp::channel& channel
		, tdk::buffer::memory_block& mb
		, void* ctx );
public:
	event_loop( void );
	~event_loop( void );

	bool open( void );
	void close( void );
	
	io_engine& engine( void );

	void set_on_accept( on_accept cb , void* ctx );
	void set_on_recv( on_recv cb , void* ctx );

	void run( void );
	bool run_once( const tdk::time_span& wait = tdk::time_span::infinite());
public:
	void on_connect_complete( void );
	void on_send_complete( void );

	void on_accept_complete( const tdk::error_code& code
		, tdk::network::tcp::acceptor& acceptor
		, tdk::network::socket fd );

	void on_connect_complete( const tdk::error_code& code
		, tdk::network::tcp::channel& channel );

	void on_recv_complete( const tdk::error_code& code
		, tdk::network::tcp::channel& channel
		, tdk::buffer::memory_block& mb );

	void on_send_complete( const tdk::error_code& code
		, tdk::network::tcp::channel& channel
		, int io_byte  );
private:
	io_engine _engine;
	std::pair< on_accept , void* > _on_accept;
	std::pair< on_recv , void* > _on_recv;
};

}}

#endif