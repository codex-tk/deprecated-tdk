#ifndef __tdk_io_engine_h__
#define __tdk_io_engine_h__

#include <tdk/io/completion_port_win32.hpp>
#include <tdk/io/operation.hpp>
#include <tdk/threading/spin_lock.hpp>
#include <tdk/util/list_node.hpp>
#include <tdk/io/engine_detail.hpp>
#include <tdk/util/rc_ptr.hpp>

namespace tdk {
namespace io {

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
private:
	class scheduler;

	class timer_operation : public operation 
		, public tdk::rc_ptr_base< timer_operation >
	{
	public:
		timer_operation( operation::callback cb 
			, const tdk::date_time& expired );
		virtual ~timer_operation( void );

		bool end_opearation();
						
		void expired_at( const tdk::date_time& at );
		const tdk::date_time& expired_at(void);
	private:
		tdk::date_time _expired_at;
	};
	typedef tdk::rc_ptr< timer_operation > timer_id;
public:
	template < typename T_handler >
	timer_id schedule( const tdk::date_time& dt 
		, const T_handler& h )
	{
		typedef tdk::io::detail::operation_impl< T_handler 
		, tdk::io::engine::timer_operation
		, detail::dispatcher_error_code
		, detail::release_policy_release
		> impl;
		timer_id id( new impl( h , dt ));
		return schedule(id);
	}
	timer_id schedule( timer_id& id );
	void cancel( timer_id& id );	
private:
	completion_port _port;
};

}}

#endif