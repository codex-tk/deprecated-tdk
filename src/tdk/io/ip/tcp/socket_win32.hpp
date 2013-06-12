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
	class connect_operation_impl : public tdk::io::ip::tcp::connect_operation {
	public:
		connect_operation_impl( tdk::io::ip::tcp::socket& fd 
			, const std::vector< tdk::io::ip::address >& addr 
			, const T_handler& h ) 
			: _handler( h ) 
			, connect_operation( fd 
				, addr 
				, &connect_operation_impl::_on_complete ) 
		{
		}
		~connect_operation_impl(void){
		}

		void on_complete( void ){
			if ( end_opearation() ) {
				_handler( error() );
				delete this;
			}
		}

		static void __stdcall _on_complete( operation* op ) {
			connect_operation_impl* impl( static_cast< connect_operation_impl* >(op));
			impl->on_complete();
		}
	private:
		T_handler _handler;
	};
	connect_operation_impl* impl( new connect_operation_impl( *this , addr , h ));
	impl->process();
}


template < typename T_handler >
void socket::async_send( const tdk::io::buffer_adapter& buf 
		, const T_handler& h )
{
	class send_operation_impl : public tdk::io::ip::tcp::send_operation {
	public:
		send_operation_impl( tdk::io::ip::tcp::socket& fd 
			, const tdk::io::buffer_adapter& buf
			, const T_handler& h ) 
			: _handler( h ) 
			, send_operation( fd 
				, buf 
				, &send_operation_impl::_on_complete ) 
		{
		}
		~send_operation_impl(void){
		}

		void on_complete( void ){
			if ( end_opearation() ) {
				_handler( error() , io_bytes() );
				delete this;
			}
		}

		static void __stdcall _on_complete( operation* op ) {
			send_operation_impl* impl( static_cast< send_operation_impl* >(op));
			impl->on_complete();
		}
	private:
		T_handler _handler;
	};
	send_operation_impl* impl( new send_operation_impl( *this , buf , h ));
	impl->process();
}

template < typename T_handler >
void socket::async_recv( const tdk::io::buffer_adapter& buf 
		, const T_handler& h )
{
	class recv_operation_impl : public tdk::io::ip::tcp::recv_operation {
	public:
		recv_operation_impl( tdk::io::ip::tcp::socket& fd 
			, const tdk::io::buffer_adapter& buf
			, const T_handler& h ) 
			: _handler( h ) 
			, recv_operation( fd 
				, buf 
				, &recv_operation_impl::_on_complete ) 
		{
		}
		~recv_operation_impl(void){
		}

		void on_complete( void ){
			if ( end_opearation() ) {
				_handler( error() , io_bytes() );
				delete this;
			}
		}

		static void __stdcall _on_complete( operation* op ) {
			recv_operation_impl* impl( static_cast< recv_operation_impl* >(op));
			impl->on_complete();
		}
	private:
		T_handler _handler;
	};
	recv_operation_impl* impl( new recv_operation_impl( *this , buf , h ));
	impl->process();
}


template < typename T_handler >
void socket::async_accept( socket& fd , const T_handler& h ){
	class accept_operation_impl : public tdk::io::ip::tcp::accept_operation {
	public:
		accept_operation_impl( 
			tdk::io::ip::tcp::socket& listen_fd 
			, tdk::io::ip::tcp::socket& fd 
			, const T_handler& h ) 
			: _handler( h ) 
			, accept_operation(  listen_fd , fd	, &accept_operation_impl::_on_complete ) 
		{
		}
		~accept_operation_impl(void){
		}

		void on_complete( void ){
			if ( end_opearation() ) {
				_handler( error() );
				delete this;
			}
		}

		static void __stdcall _on_complete( operation* op ) {
			accept_operation_impl* impl( static_cast< accept_operation_impl* >(op));
			impl->on_complete();
		}
	private:
		T_handler _handler;
	};
	accept_operation_impl* impl( new accept_operation_impl( *this , fd , h ));
	impl->process();
}

}}}}

#endif