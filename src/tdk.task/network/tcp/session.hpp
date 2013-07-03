#ifndef __tdk_network_tcp_session_h__
#define __tdk_network_tcp_session_h__

#include <tdk.task/network/tcp/channel.hpp>

namespace tdk {
namespace network {
namespace tcp {
	
class session;
class session_handler {
public:
	session_handler(void){}
	virtual ~session_handler ( void ){}
	virtual void on_open( session* s );
	virtual void on_recv( session* s , tdk::buffer::memory_block& mb ) = 0;
	virtual void on_send( session* s , int sent_bytes , int remain_bytes ) = 0;
	virtual void on_error( session* s , const std::error_code& code ) = 0;
	virtual void on_close( session* s ) = 0;
};

class session : public tdk::task::operation {
public:
	session ( task::event_loop& loop );
	~session ( void );

	// for accept
	bool open( tdk::network::socket& fd  , session_handler* handler );
	// for connect
	bool open( session_handler* handler );

	void close();

private:
	tdk::network::tcp::channel _channel;
	tdk::network::tcp::recv_operation* _recv_op;
	tdk::network::tcp::send_operation* _send_op;

	session_handler* _handler;

	bool _sending;
	std::vector< tdk::buffer::memory_block > _send_buffer;
};

}}}

#endif