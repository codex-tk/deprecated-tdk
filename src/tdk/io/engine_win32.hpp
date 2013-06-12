#ifndef __tdk_io_engine_h__
#define __tdk_io_engine_h__

#include <tdk/io/completion_port_win32.hpp>
#include <tdk/io/operation.hpp>

namespace tdk {
namespace io {
namespace ip{
namespace tcp{

class connect_operation;
class send_operation;
class recv_operation;
class accept_operation;

}}
class engine {
public:
	engine( void );
	~engine( void );

	bool open( void );
	void close( void );

	bool run( const tdk::time_span& wait );
	 
	bool post( tdk::io::operation* op , const tdk::error_code& ec );
	void async_connect( tdk::io::ip::tcp::connect_operation* op );
	void async_send( tdk::io::ip::tcp::send_operation* op );
	void async_recv( tdk::io::ip::tcp::recv_operation* op );
	void async_accept( tdk::io::ip::tcp::accept_operation* op );

	bool bind( SOCKET fd );

	template < typename T_op > 
	void error_handler( T_op* op ) {
		op->socket().close();
		post( op , tdk::platform::error() );
	}
private:
	completion_port _port;
};

}}

#endif