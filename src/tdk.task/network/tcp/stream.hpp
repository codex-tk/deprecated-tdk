#ifndef __tdk_network_tcp_stream_h__
#define __tdk_network_tcp_stream_h__

#include <tdk.task/network/tcp/channel.hpp>
#include <tdk/threading/spin_lock.hpp>
namespace tdk {
namespace network {
namespace tcp {

class stream;
class stream_handler {
public:
	stream_handler(void){}
	virtual ~stream_handler ( void ){}
	virtual void on_recv( stream& s , tdk::buffer::memory_block& mb ) = 0;
	virtual void on_send( stream& s , int sent_bytes , int remain_bytes ) = 0;
	virtual void on_error( stream& s , const tdk::error_code& code ) = 0;
	virtual void on_close( stream& s ) = 0;
};

class stream {
public:
	stream ( task::event_loop& loop );
	~stream ( void );
	// for accept
	bool open( tdk::network::socket& fd  , stream_handler* handler );
	// for connect
	bool open( stream_handler* handler );

	void close();
	
	void recv( const tdk::buffer::memory_block& mb );
	void send( const tdk::buffer::memory_block& mb );
	
	void reset( void );

	stream_handler* handler( void );

	tdk::network::tcp::channel& channel(void);

	void* tag( void );
	void tag( void* p );
private:
	bool _is_closed(void);
	void _internal_close( const tdk::error_code& code , bool closed = false );
	bool _post_error( const tdk::error_code& code );
	void _post_close( void );
	void _on_recv( tdk::network::tcp::recv_operation& r );
	void _on_send( tdk::network::tcp::send_operation& r );
private:
	tdk::threading::spin_lock _lock;
	tdk::network::tcp::channel _channel;
	tdk::network::tcp::recv_operation* _recv_op;
	tdk::network::tcp::send_operation* _send_op;
	stream_handler* _handler;
	void* _tag;

	bool _sending;
	std::vector< tdk::buffer::memory_block > _send_buffer;
	
	std::atomic<int> _ref_count;

	enum class post_status {
		not_post ,
		begin_post ,
		end_post ,
	};
	post_status _error_post_status;
	post_status _close_post_status;

	bool _closed;	
};

}}}

#endif