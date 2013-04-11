#ifndef __tdk_network_tcp_connector_h__
#define __tdk_network_tcp_connector_h__

#include <tdk.task/network/tcp/channel.hpp>
#include <tdk.task/network/tcp/operation/connect_operation.hpp>

namespace tdk {
namespace network {
namespace tcp {

class connector {
public:
	template < typename T_handler >
	void connect( tdk::network::tcp::channel* ch 
		, const tdk::network::address& addr 
		, const T_handler& handler )
	{
		class connect_operation_impl : public tdk::network::tcp::connect_operation{
		public:
			connect_operation_impl( tdk::network::tcp::channel* c 
				, const tdk::network::address& addr
				, const T_handler& handler  )
				: connect_operation( c , addr )
				, _handler( handler )
			{
			}
			virtual ~connect_operation_impl( void ){

			}
			virtual void operator()(void ){
				_handler( *this );
				delete this;
			}
		private:
			T_handler _handler;
		};
		return connect( new connect_operation_impl( ch , addr , handler ));
	}

	void connect( tdk::network::tcp::connect_operation* op );
};

}}}


#endif