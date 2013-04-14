#ifndef __tdk_network_udp_channel_h__
#define __tdk_network_udp_channel_h__

#include <tdk.task/task/event_loop_object.hpp>
#include <tdk/network/socket.hpp>
#include <tdk/buffer/memory_block.hpp>
#include <tdk.task/network/udp/operation/recvfrom_operation.hpp>

namespace tdk {
namespace network {
namespace udp {

class channel : public task::event_loop_object{
public:
	channel( tdk::task::event_loop& loop );
	
	bool open( int af = AF_INET );
	bool bind( const tdk::network::address& addr );
	void close( void );

	int sendto( const tdk::buffer::memory_block& mb 
		, const tdk::network::address& addr );

	void recvfrom( recvfrom_operation* op );

	tdk::network::socket& socket( void );
private:
	tdk::network::socket _fd;
public:
	template < typename T_handler >
	void recvfrom( tdk::buffer::memory_block& mb  
		, const T_handler& handler ) {
		class recvfrom_operation_impl : public tdk::network::udp::recvfrom_operation{
		public:
			recvfrom_operation_impl( tdk::network::udp::channel* c 
				, tdk::buffer::memory_block& mb
				, const T_handler& handler  )
				: recvfrom_operation( c , mb )
				, _handler( handler )
			{
			}
			virtual ~recvfrom_operation_impl( void ){

			}
			virtual void operator()(void ){
				before_dispatch();
				_handler( *this );
				after_dispatch();
				delete this;
			}
		private:
			T_handler _handler;
		};
		return recvfrom( new recvfrom_operation_impl( this , mb , handler ));
	}
};

}}}

#endif