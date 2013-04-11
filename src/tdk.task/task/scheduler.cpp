#include "stdafx.h"
#include <tdk.task/task/scheduler.hpp>
#include <tdk.task/task/event_loop.hpp>
#include <tdk/error/platform_error.hpp>

namespace tdk {
namespace task {

scheduler::scheduler( event_loop& loop ) 
	: _loop( loop )
{
}

scheduler::~scheduler( void ){
}

void scheduler::schedule( const timer_id& id ) {
	tdk::threading::scoped_lock<> guard( _lock );
	std::list< timer_id >::iterator it =
		std::upper_bound( _timers.begin() , _timers.end() , id , 
		[]( const timer_id& value , const timer_id& compare )->bool{
			  return value->expired_at() < compare->expired_at();
		});
	_timers.insert( it , id );
	_loop.increment_ref();
}

tdk::time_span scheduler::next_schedule(void) {
	tdk::threading::scoped_lock<> guard( _lock );
	tdk::date_time local = tdk::date_time::local();
	if ( _timers.empty() ||  
		((*_timers.begin())->expired_at() <= local )) {
		return tdk::time_span::from_milli_seconds(100);
	}
	return (*_timers.begin())->expired_at() - local;
}

void scheduler::drain( void ) {
	tdk::threading::scoped_lock<> guard( _lock );
	tdk::date_time local = tdk::date_time::local();
	while ( !_cancelled.empty() ) {
		timer_id id = _cancelled.front();
		_cancelled.pop_front();
		id->error( tdk::platform_error(ERROR_OPERATION_ABORTED));
		(*id)();
		_loop.decrement_ref();
	}
	while ( !_timers.empty() ) {
		if ( (*_timers.begin())->expired_at() <= local ){
			timer_id id = _timers.front();
			_timers.pop_front();
			id->error( tdk::platform_error(ERROR_SUCCESS));
			(*id)();
			_loop.decrement_ref();
		}else{
			break;
		}
	}
}

bool scheduler::cancel( const timer_id& id ) {
	tdk::threading::scoped_lock<> guard( _lock );
	auto it = std::find( _timers.begin() , _timers.end() , id );
	if( it != _timers.end() ) {
		_cancelled.push_back( *it );
		_timers.erase( it );
		return true;
	}
	return false;
}

}}