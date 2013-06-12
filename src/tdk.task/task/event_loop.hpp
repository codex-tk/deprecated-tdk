#ifndef __tdk_event_loop_h__
#define __tdk_event_loop_h__

#include <tdk.task/task/win32/io_engine.hpp>
#include <tdk.task/network/tcp/acceptor.hpp>
#include <tdk.task/network/tcp/channel.hpp>
#include <tdk.task/task/scheduler.hpp>

namespace tdk {
namespace task {

class event_loop{
public:
	event_loop( void );
	~event_loop( void );

	bool open( void );
	void close( void );
	
	io_engine& engine( void );
	tdk::task::scheduler& scheduler( void );

	void run( void );
	bool run_once( const tdk::time_span& wait = tdk::time_span::infinite());

	void post( tdk::task::operation* op);

	static event_loop* current( void );
public:
	void increment_ref( void );
	void decrement_ref( void );

	static event_loop& default_loop( void );
private:
	io_engine _engine;
	tdk::task::scheduler _scheduler;
	tdk::threading::atomic<int> _ref;

public:
	template < typename T_handler >
	void post( const T_handler& handler ) {
		class operation_impl : public tdk::task::operation {
		public:
			operation_impl( const T_handler& h ) 
				: _handler( h ) {
			}
			virtual ~operation_impl( void ) {
			}

			virtual void operator()( void ) {
				_handler();
				delete this;
			}
		private:
			T_handler _handler;
		};
		tdk::task::operation* op = new operation_impl( handler );
		post( op );
	}
};

}}

#endif
