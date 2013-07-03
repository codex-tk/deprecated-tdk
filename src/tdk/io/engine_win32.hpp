#ifndef __tdk_io_engine_h__
#define __tdk_io_engine_h__

#include <atomic>

#include <tdk/io/completion_port_win32.hpp>
#include <tdk/io/operation.hpp>
#include <tdk/io/engine_detail.hpp>

#include <tdk/threading/spin_lock.hpp>

#include <tdk/util/list_node.hpp>
#include <tdk/util/rc_ptr.hpp>

namespace tdk {
namespace io {

class engine {
public:
	engine( void );
	~engine( void );

	bool run_once( const tdk::time_span& wait );
	
	void run( void );
	 
	void post( tdk::io::operation* op , const std::error_code& ec );
	void async_connect( tdk::io::ip::tcp::connect_operation* op );
	void async_send( tdk::io::ip::tcp::send_operation* op );
	void async_recv( tdk::io::ip::tcp::recv_operation* op );
	void async_accept( tdk::io::ip::tcp::accept_operation* op );

	bool bind( SOCKET fd );
	bool post0( tdk::io::operation* op , const std::error_code& ec );
	
	void process( const std::error_code& code , int io_byte  , OVERLAPPED* ov );

	void inc_posted( void );
	void dec_posted( void );

	//void _post( tdk::io::operation* op );
public:
	static void set_exception_handler( LONG ( __stdcall* exception_handler)( EXCEPTION_POINTERS*  ) );
private:
	class scheduler;
	class scheduler_timer;
	class timer_operation : public operation 
		, public tdk::rc_ptr_base< timer_operation >
	{
	public:
		timer_operation( operation::callback cb 
			, const tdk::date_time& expired );
		virtual ~timer_operation( void );

		bool end_operation();
						
		void expired_at( const tdk::date_time& at );
		const tdk::date_time& expired_at(void);
	private:
		tdk::date_time _expired_at;
	};
public:
	typedef tdk::rc_ptr< timer_operation > timer_id;

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
	scheduler* _scheduler;
	std::atomic<int> _posted;
	//tdk::threading::atomic<int> _posted;
};

}}

#endif