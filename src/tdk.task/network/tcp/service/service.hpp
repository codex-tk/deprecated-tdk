#ifndef __tdk_network_tcp_server_service_h__
#define __tdk_network_tcp_server_service_h__

#include <tdk.task/task/event_loop_object.hpp>
#include <tdk.task/network/tcp/acceptor.hpp>

namespace tdk {
namespace network {
namespace tcp{

class session;

class service_handler {
public:
	service_handler( void ){}
	virtual ~service_handler ( void ){}

	virtual void on_accpet( session* s ) = 0;
	virtual void on_connect_success( session* s , const tdk::network::address& addr ) = 0;
	virtual void on_connect_fail( const tdk::network::address& addr )= 0;
	virtual void on_recv( session* s ) = 0;
	virtual void on_sendable( session* s ) = 0;
	virtual void on_close( session* s )=0;
};

class service : public tdk::task::event_loop_object{
public:
	service( tdk::task::event_loop& loop );
	~service( void );

	void set_handler( service_handler* handler );

	bool start_server( const tdk::network::address& addr );
	void stop_server( void );

private:
	tdk::network::tcp::acceptor _acceptor;
	service_handler* _handler;
};

}}}


#endif