#ifndef __tdk_io_engine_h__
#define __tdk_io_engine_h__

#include <tdk/io/completion_port_win32.hpp>
#include <tdk/io/operation.hpp>
#include <tdk/threading/spin_lock.hpp>
#include <tdk/util/list_node.hpp>

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
	 
	void post( tdk::io::operation* op , const tdk::error_code& ec );
	void async_connect( tdk::io::ip::tcp::connect_operation* op );
	void async_send( tdk::io::ip::tcp::send_operation* op );
	void async_recv( tdk::io::ip::tcp::recv_operation* op );
	void async_accept( tdk::io::ip::tcp::accept_operation* op );

	bool bind( SOCKET fd );

	void on_timer( void );
	void drain( void );

	template < typename T_op > 
	void error_handler( T_op* op ) {
		op->socket().close();
		post( op , tdk::platform::error() );
	}
private:
	class timer_opeartion : public tdk::io::operation {
	public:
		timer_opeartion( engine& e ) ;
		~timer_opeartion( void );
		void on_complete( void );
		static void __stdcall _on_complete( tdk::io::operation* op );
	private:
		engine& _engine;
	};
private:
	completion_port _port;
	// for post fails
	tdk::threading::spin_lock _lock;
	tdk::slist_queue< tdk::io::operation > _op_queue;

	timer_opeartion* _timer_op;
	HANDLE _timer_queue;
	bool _timer_in_progress;
};

}}

#endif