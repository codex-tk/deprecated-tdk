#ifndef __tdk_network_tcp_channel_h__
#define __tdk_network_tcp_channel_h__

#include <tdk.task/task/event_loop_object.hpp>
#include <tdk.task/network/tcp/operation/recv_operation.hpp>
#include <tdk.task/network/tcp/operation/send_operation.hpp>
#include <tdk/network/socket.hpp>
#include <tdk/buffer/memory_block.hpp>

namespace tdk {
namespace network {
namespace tcp {

class channel : public task::event_loop_object{
public:
	channel( tdk::task::event_loop& loop );
	
	bool open( tdk::network::socket& fd );
	void close( void );

	template < typename T_handler >
	void recv( tdk::buffer::memory_block& mb  
		, const T_handler& handler ) {
		class recv_operation_impl : public tdk::network::tcp::recv_operation{
		public:
			recv_operation_impl( tdk::network::tcp::channel* c 
				, tdk::buffer::memory_block& mb
				, const T_handler& handler  )
				: recv_operation( c , mb )
				, _handler( handler )
			{
			}
			virtual ~recv_operation_impl( void ){

			}
			virtual void operator()(void ){
				recv_operation::operator()();
				_handler( *this );
				delete this;
			}
		private:
			T_handler _handler;
		};
		return recv( new recv_operation_impl( this , mb , handler ));
	}

	void recv( recv_operation* op );


	template < typename T_handler >
	void send( std::vector<tdk::buffer::memory_block>& mb  
		, const T_handler& handler ) {
		class send_operation_impl : public tdk::network::tcp::send_operation{
		public:
			send_operation_impl( tdk::network::tcp::channel* c 
				, std::vector<tdk::buffer::memory_block>& mb
				, const T_handler& handler  )
				: send_operation( c , mb )
				, _handler( handler )
			{
			}
			virtual ~send_operation_impl( void ){

			}
			virtual void operator()(void ){
				send_operation::operator()();
				_handler( *this );
				delete this;
			}
		private:
			T_handler _handler;
		};
		return send( new send_operation_impl( this , mb , handler ));
	}

	template < typename T_handler >
	void send( tdk::buffer::memory_block& mb  
		, const T_handler& handler ) {
		std::vector< tdk::buffer::memory_block > mbs;
		mbs.push_back( mb );
		send( mbs , handler );
	}

	void send( send_operation* op );

	tdk::network::socket& socket( void );

private:
	tdk::network::socket _fd;
};

}}}


#endif