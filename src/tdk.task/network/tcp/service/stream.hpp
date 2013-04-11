#ifndef __tdk_network_tcp_stream_h__
#define __tdk_network_tcp_stream_h__

#include <tdk.task/network/tcp/channel.hpp>

namespace tdk {
namespace network {
namespace tcp {

class stream;
class stream_handler {
public:
	stream_handler(void){}
	virtual ~stream_handler ( void ){}
	virtual void on_recv( stream* s , tdk::buffer::memory_block& mb ) = 0;
	virtual void on_send( stream* s , int send_bytes , int remain_bytes ) = 0;
	virtual void on_close( stream* s , const tdk::error_code& code ) = 0;
};

class stream {
public:
	stream ( task::event_loop& loop );
	~stream ( void );
	// for accept
	bool open( tdk::network::socket& fd  , stream_handler* handler );
	// for connect
	bool open( stream_handler* handler );

	void close( void );
	void recv( const tdk::buffer::memory_block& mb );
	void send( const tdk::buffer::memory_block& mb );
private:
	void _on_recv( tdk::network::tcp::recv_operation& r );
	void _on_send( tdk::network::tcp::send_operation& r );
private:
	tdk::network::tcp::channel _channel;
	tdk::network::tcp::recv_operation* _recv_op;
	tdk::network::tcp::send_operation* _send_op;
	stream_handler* _handler;
	int _io_ref;
	bool _closed;
	std::vector< tdk::buffer::memory_block > _send_buffer;
};

}}}

#endif