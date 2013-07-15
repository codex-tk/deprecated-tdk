#ifndef __tdk_io_ip_tcp_socket_h__
#define __tdk_io_ip_tcp_socket_h__

#include <tdk/io/ip/socket.hpp>
#include <tdk/io/engine_win32.hpp>
#include <tdk/io/buffer_adapter.hpp>
#include <tdk/io/ip/tcp/operation/connect_operation_win32.hpp>
#include <tdk/io/ip/tcp/operation/send_operation_win32.hpp>
#include <tdk/io/ip/tcp/operation/recv_operation_win32.hpp>
#include <tdk/io/ip/tcp/operation/accept_operation_win32.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class socket : public tdk::io::ip::socket {
public:
	socket( tdk::io::engine& engine );
	~socket( void );
	tdk::io::engine& engine( void );

	template < typename T_handler >
	void async_connect( 
		const tdk::io::ip::address& addr 
		, const T_handler& h )
	{
		std::vector< tdk::io::ip::address > addrs;
		addrs.push_back( addr );
		async_connect( addrs , h );
	}

	template < typename T_handler >
	void async_connect( 
		const std::vector< tdk::io::ip::address >& addr 
		, const T_handler& h ); 

	template < typename T_handler >
	void async_send( const tdk::io::buffer_adapter& buf 
		, const T_handler& h );

	template < typename T_handler >
	void async_recv( const tdk::io::buffer_adapter& buf 
		, const T_handler& h );

	template < typename T_handler >
	void async_accept( socket& fd ,  const T_handler& h );
private:
	tdk::io::engine* _engine;
};

template < typename T_handler >
void socket::async_connect( const std::vector< tdk::io::ip::address >& addr 
						   , const T_handler& h ) 
{
	typedef tdk::io::detail::operation_impl< T_handler 
		, tdk::io::ip::tcp::connect_operation 
		, detail::dispatcher_error_code
	> connect_operation_impl;
	connect_operation_impl* impl( new connect_operation_impl( h , *this , addr ));
	impl->process();
}

template < typename T_handler >
void socket::async_send( const tdk::io::buffer_adapter& buf 
		, const T_handler& h )
{
	typedef tdk::io::detail::operation_impl< T_handler 
		, tdk::io::ip::tcp::send_operation 
		, detail::dispatcher_error_code_and_io_bytes
		> send_operation_impl;
	send_operation_impl* impl( new send_operation_impl( h , *this , buf ));
	impl->process();
}

template < typename T_handler >
void socket::async_recv( const tdk::io::buffer_adapter& buf 
		, const T_handler& h )
{
	typedef tdk::io::detail::operation_impl< T_handler 
		, tdk::io::ip::tcp::recv_operation
		, detail::dispatcher_error_code_and_io_bytes
		> recv_operation_impl;
	recv_operation_impl* impl( new recv_operation_impl( h , *this , buf ));
	impl->process();
}

template < typename T_handler >
void socket::async_accept( socket& fd , const T_handler& h ){
	typedef tdk::io::detail::operation_impl< T_handler
		, tdk::io::ip::tcp::accept_operation 
		, detail::dispatcher_error_code  > accept_operation_impl;
	accept_operation_impl* impl( new accept_operation_impl( h , *this , fd ));
	impl->process();
}






}}}}

#endif
