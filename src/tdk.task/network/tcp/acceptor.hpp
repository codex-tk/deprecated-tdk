#ifndef __tdk_network_tcp_acceptor_h__
#define __tdk_network_tcp_acceptor_h__

#include <tdk.task/task/event_loop_object.hpp>
#include <tdk/network/socket.hpp>
#include <tdk.task/network/tcp/operation/accept_operation.hpp>

namespace tdk {
namespace network {
namespace tcp {

class acceptor: public task::event_loop_object{
public:
	acceptor( tdk::task::event_loop& loop );

	bool open( const tdk::network::address& addr );
	void close( void );
	/*
	void accept( void );
	*/
	template < typename T_handler >
	void accept( const T_handler& handler ) {
		class accept_operation_impl : public tdk::network::tcp::accept_operation{
		public:
			accept_operation_impl( tdk::network::tcp::acceptor& a , const T_handler& handler  )
				: accept_operation( a )
				, _handler( handler )
			{
			}
			virtual ~accept_operation_impl( void ){

			}
			virtual void operator()(void ){
				_handler( *this );
				delete this;
			}
		private:
			T_handler _handler;
		};
		accept_operation* op = new accept_operation_impl( *this , handler );
		return accept( op );
	}
	
	void accept( accept_operation* op );
	
	tdk::network::socket& socket( void );
	const tdk::network::address& address( void );
private:
	tdk::network::socket _fd;
	tdk::network::address _address;
};

}}}


#endif