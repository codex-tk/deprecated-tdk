#include "stdafx.h"
#include <tdk/io/engine_scheduler_win32.hpp>
#include <tdk/task/queue_timer_win32.hpp>

namespace tdk {
namespace io {

engine::timer_operation::timer_operation( operation::callback cb 
	, const tdk::date_time& expired )
	: operation( cb )
	, _expired_at( expired )
{
	// release 는 operation_impl 의 release_policy_release 에서
	retain();
}

engine::timer_operation::~timer_operation( void ) {

}

bool engine::timer_operation::end_operation() {
	return true;
}
					
void engine::timer_operation::expired_at( const tdk::date_time& at ) {
	_expired_at = at;
}

const tdk::date_time& engine::timer_operation::expired_at(void) {
	return _expired_at;
}

engine::scheduler::scheduler( tdk::io::engine& e  ) 
	: operation( &scheduler::_on_complete )
	, _engine(e)
	, _in_progress( false )
	, _closed( false )
{
	retain();	
}

engine::scheduler::~scheduler( void ) {

}

void engine::scheduler::open( void ) {
	_enable_schedule();
}

void engine::scheduler::close( void ) {
	_closed = true;
	tdk::task::timer_id id( this );
	tdk::task::queue_timer::instance().cancel( id );
}

void engine::scheduler::schedule( tdk::io::engine::timer_id& id ) {
	tdk::threading::scoped_lock<> guard( _lock );	
	std::list< timer_id >::iterator it =
		std::upper_bound( _timers.begin() , _timers.end() , id , 
		[]( const timer_id& value , const timer_id& compare )->bool{
			  return value->expired_at() < compare->expired_at();
		});
	_timers.insert( it , id );
}

void __stdcall engine::scheduler::_on_complete( operation* op ) {
	scheduler* sch( static_cast< scheduler* >(op));
	sch->drain();
}

bool engine::scheduler::cancel( tdk::io::engine::timer_id& id ) {
	tdk::threading::scoped_lock<> guard( _lock );
	auto it = std::find( _timers.begin() , _timers.end() , id );
	if( it != _timers.end() ) {
		_cancels.push_back( *it );
		_timers.erase( it );
		return true;
	}
	return false;
}

void engine::scheduler::drain( void ) {
	drain_post_fails();
	drain_cancels();
	drain_expired();
	tdk::threading::scoped_lock<> guard( _lock );
	_in_progress = false;
}

void engine::scheduler::post_fail( operation* op ) {
	tdk::threading::scoped_lock<> guard( _lock );
	_op_queue.add_tail( op );
}

void engine::scheduler::drain_post_fails( void ) {
	while (true) {
		tdk::io::operation* op = nullptr;
		do {
			tdk::threading::scoped_lock<> gaurd( _lock );
			if ( _op_queue.is_empty() ) 
				return;			
			 op = _op_queue.front();
			 _op_queue.pop_front();
		}while(0);
		(*op)( op->error() , op->io_bytes() );
	}
}

void engine::scheduler::drain_cancels( void ) {
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
		(*id)( tdk::tdk_user_abort , 0 );
	}
}

void engine::scheduler::drain_expired( void ) {
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
		(*id)( tdk::tdk_success , 0 );
	}
}

void engine::scheduler::operator()( const tdk::error_code& e){
	bool post = false;
	do {
		tdk::threading::scoped_lock<> gaurd( _lock );
		if ( !_in_progress ) {
			if ( !_op_queue.is_empty() || !_cancels.empty() )
			{
				post = true;
			} else {
				if ( !_timers.empty() ){
					if ( _timers.front()->expired_at() <= tdk::date_time::local() ) {
						post = true;
					}
				}
			}			
		}	
	}while(0);

	if ( post ) {
		if ( _engine.post0( this , tdk::tdk_success ) ){
			tdk::threading::scoped_lock<> guard( _lock );
			_in_progress = true;
		}
	}	
	_enable_schedule();
}

void engine::scheduler::_enable_schedule( void ) {
	if ( _closed )
		return;

	tdk::task::timer_id id( this );
	id->expired_at( tdk::date_time::local() + tdk::time_span::from_seconds(1));
	tdk::task::queue_timer::instance().schedule( id );
}

}}