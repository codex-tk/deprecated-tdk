#include "stdafx.h"
#include <tdk.task/task/event_loop.hpp>

namespace tdk {
namespace task {

event_loop::event_loop()
	: _engine( *this )
	, _scheduler(*this)
{
}

event_loop::~event_loop(){

};

bool event_loop::open( void ){
	return _engine.open();
}

void event_loop::close( void ) {
	_engine.close();
}

io_engine& event_loop::engine( void ){ 
	return _engine;
}

void event_loop::run( void ) {
	while ( _ref.compare_and_swap( 1 , 1 ) != 0 ) {
		_engine.run( _scheduler.next_schedule() );
		_scheduler.drain();
	}
}

bool event_loop::run_once( const tdk::time_span& wait ) {
	if ( _ref.compare_and_swap( 1 , 1 ) == 0 )
		return false;
	_engine.run( _scheduler.next_schedule() );
	_scheduler.drain();
	return true;
}

tdk::task::scheduler& event_loop::scheduler( void ) {
	return _scheduler;
}

event_loop& event_loop::default_loop( void ) {
	static event_loop loop;
	return loop;
}

void event_loop::increment_ref( void ) {
	_ref.increment();
}

void event_loop::decrement_ref( void ) {
	_ref.decrement();
}


}}
