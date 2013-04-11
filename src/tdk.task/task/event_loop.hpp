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

public:
	void increment_ref( void );
	void decrement_ref( void );

	static event_loop& default_loop( void );
private:
	io_engine _engine;
	tdk::task::scheduler _scheduler;
	tdk::threading::atomic<int> _ref;
};

}}

#endif
