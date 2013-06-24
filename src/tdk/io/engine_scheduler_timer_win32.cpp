#include "stdafx.h"
#include <tdk/io/engine_scheduler_timer_win32.hpp>
#include <tdk/io/engine_scheduler_win32.hpp>

namespace tdk {
namespace io {
	
void engine::scheduler_timer::on_timer( void* p , BOOLEAN timer_or_wait_fired ) {
 	engine::scheduler_timer* st( static_cast< engine::scheduler_timer* >( p ));
	st->tick();
}

engine::scheduler_timer::scheduler_timer( void ) {
	if ( !CreateTimerQueueTimer( &_timer
 		, nullptr 
 		, &engine::scheduler_timer::on_timer 
 		, this 
 		, 0
 		, 500  
 		, WT_EXECUTEDEFAULT ))
 	{
 	}
}

engine::scheduler_timer::~scheduler_timer( void ) {
	DeleteTimerQueueTimer( nullptr , _timer , INVALID_HANDLE_VALUE );
}

void engine::scheduler_timer::reg( engine::scheduler* sch ) {
	tdk::threading::scoped_lock<> gaurd(_lock);
	_scheduler.insert( sch );
}

void engine::scheduler_timer::unreg( engine::scheduler* sch ) {
	tdk::threading::scoped_lock<> gaurd(_lock);
	_scheduler.erase( sch );
}

void engine::scheduler_timer::tick( void ) {
	tdk::threading::scoped_lock<> gaurd(_lock);
	tdk::date_time now = tdk::date_time::local();
	for ( engine::scheduler* sch : _scheduler ) {
		if ( now > sch->schedule_time() ) {
			sch->on_timer();
		}
	}
}



}}