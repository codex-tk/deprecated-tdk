#include "stdafx.h"
#include <tdk/util/singleton.hpp>
#include <tdk/error/error_tdk.hpp>
#include <tdk/io/engine_epoll.hpp>
#include <sys/epoll.h>
#include <algorithm>

namespace tdk {
namespace io {
namespace detail{

static void event_callback( void* obj , int evt ) {
    tdk::io::engine::scheduler* sch = 
        static_cast< tdk::io::engine::scheduler*>( obj );
    sch->handle_event(evt);
}

}
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
	: _engine(e)
{
    _context.object = this;
    _context.callback = &detail::event_callback;
    _engine.ctl( EPOLL_CTL_ADD 
            , _timerfd.handle()
            , EPOLLIN | EPOLLET | EPOLLONESHOT 
            , &_context );
}

engine::scheduler::~scheduler( void ) {
    _engine.ctl( EPOLL_CTL_DEL
            , _timerfd.handle()
            , 0
            , nullptr);
}

void engine::scheduler::post( tdk::io::operation* op ) {
    tdk::threading::scoped_lock<> gaurd( _lock );
    _op_queue.add_tail( op );
    _timerfd.set( tdk::date_time::utc());
    _engine.inc_posted();
}

tdk::io::operation* engine::scheduler::fetch( void ) {
    tdk::threading::scoped_lock<> gaurd( _lock );
    if ( _op_queue.is_empty() ) {
        return nullptr;
    } else {
        auto op = _op_queue.front();
        _op_queue.pop_front();
        return op;
    }
}

void engine::scheduler::drain_op_queue( void ) {
    while( true ) {
        tdk::io::operation* op = fetch();
        if ( op ) {
            (*op)(op->error() , op->io_bytes());
            _engine.dec_posted();        
        } else {
            break;
        }
    }
}

void engine::scheduler::drain_expired( void ) {
	tdk::date_time now = tdk::date_time::utc();
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
		_engine.dec_posted();
	}

}

void engine::scheduler::drain_cancels( void ){
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
		_engine.dec_posted();
	}
}

void engine::scheduler::handle_event( int evt ) {
    if ( evt & EPOLLIN ) {
        drain_cancels();
        drain_expired();
        drain_op_queue();
    }
    _engine.ctl( EPOLL_CTL_MOD
            , _timerfd.handle()
            , EPOLLIN | EPOLLET | EPOLLONESHOT 
            , &_context );
   
    set_wakeup_time();
}


void engine::scheduler::set_wakeup_time(void){
    tdk::threading::scoped_lock<> gaurd( _lock );
    if( !_op_queue.is_empty() || !_cancels.empty()){
        _timerfd.set( tdk::date_time::utc());
        return;
    }
    if ( _timers.empty() ) {
        _timerfd.set( tdk::date_time::utc() + tdk::time_span::from_days(1));
    } else {
        _timerfd.set( _timers.front()->expired_at());
    }
}

void engine::scheduler::schedule( tdk::io::engine::timer_id& id ) {
	tdk::threading::scoped_lock<> guard( _lock );	
	std::list< timer_id >::iterator it =
		std::upper_bound( _timers.begin() , _timers.end() , id , 
		[]( const timer_id& value , const timer_id& compare )->bool{
			  return value->expired_at() < compare->expired_at();
		});
	_timers.insert( it , id );
    set_wakeup_time();
    _engine.inc_posted();
}

bool engine::scheduler::cancel( tdk::io::engine::timer_id& id ) {
	tdk::threading::scoped_lock<> guard( _lock );
	auto it = std::find( _timers.begin() , _timers.end() , id );
	if( it != _timers.end() ) {
		_cancels.push_back( *it );
		_timers.erase( it );
        _timerfd.set( tdk::date_time::utc());
		return true;
	}
	return false;
}
}}
