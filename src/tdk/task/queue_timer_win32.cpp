#include "stdafx.h"
#include <tdk/task/queue_timer_win32.hpp>
#include <tdk/error/error_tdk.hpp>

namespace tdk {
namespace task {

queue_timer::queue_timer( void )
 	: _timer( INVALID_HANDLE_VALUE )
{
	if ( !CreateTimerQueueTimer( &_timer
 		, nullptr 
 		, &queue_timer::_on_timer 
 		, this 
 		, 0
 		, 100  
 		, WT_EXECUTEDEFAULT ))
 	{
 	}
 }

queue_timer::~queue_timer(void){
 	DeleteTimerQueueTimer( nullptr , _timer , INVALID_HANDLE_VALUE );
}

void queue_timer::schedule( const timer_id& id ) {
	tdk::threading::scoped_lock<> guard( _lock );
	std::list< timer_id >::iterator it =
		std::upper_bound( _timers.begin() , _timers.end() , id , 
		[]( const timer_id& value , const timer_id& compare )->bool{
			  return value->expired_at() < compare->expired_at();
		});
	_timers.insert( it , id );
}

bool queue_timer::cancel( const timer_id& id ) {
	tdk::threading::scoped_lock<> guard( _lock );
	auto it = std::find( _timers.begin() , _timers.end() , id );
	if( it != _timers.end() ) {
		_cancels.push_back( *it );
		_timers.erase( it );
		return true;
	}
	return false;
}

void queue_timer::drain_cancels( void ){
	tdk::date_time now = tdk::date_time::local();
	while (true) {
		timer_id id( nullptr );
		do {
			tdk::threading::scoped_lock<> gaurd( _lock );
			if ( _cancels.empty() ) 
				return;			
			id = _cancels.front();
			_cancels.pop_front();
		}while(0);
		(*id)( tdk::tdk_network_user_abort );
	}
}

void queue_timer::drain_expired( void ) {
	tdk::date_time now = tdk::date_time::local();
	while (true) {
		timer_id id( nullptr );
		do {
			tdk::threading::scoped_lock<> gaurd( _lock );
			if ( _timers.empty() ) 
				return;
			if ( _timers.front()->expired_at() > now )
				return;

			id = _timers.front();
			_timers.pop_front();
		}while(0);
		(*id)( tdk::tdk_success );
	}
}

void queue_timer::on_timer( void ){
	drain_cancels();
	drain_expired();
}

 void __stdcall queue_timer::_on_timer( void* p , BOOLEAN timer_or_wait_fired ) {
 	queue_timer* qt( static_cast< queue_timer* >( p ));
 	if ( qt ) 
 		qt->on_timer();		
 }
 /*
 queue_timer& queue_timer::instance( void ) {
 	static queue_timer timer;
 	return timer;
 }*/

}}