#ifndef __tdk_task_results_h__
#define __tdk_task_results_h__

#include <tdk/network/socket.hpp>
#include <tdk/error/error_code.hpp>
#include <tdk.task/network/tcp/acceptor.hpp>
#include <tdk.task/network/tcp/channel.hpp>

namespace tdk{
namespace task {

enum class io_event {
	accept 
	, connect
	, recv
	, send
};

struct io_context : OVERLAPPED {
	io_event event;

	void reset( void );
	tdk::error_code error( void );
	void error( const tdk::error_code& code );
};

struct accept_io_context : public io_context {
	accept_io_context( void );

	tdk::network::tcp::acceptor* acceptor;
	tdk::network::socket fd;
	sockaddr_storage address[2];
};

struct connect_io_context : public io_context {
	connect_io_context( void );
};

struct recv_io_context : public io_context {
	recv_io_context( void );
	tdk::network::tcp::channel* channel;
	tdk::buffer::memory_block recv_buffer;
};

struct send_io_context : public io_context {
	send_io_context( void );
};

}}

#endif