#include "stdafx.h"
#include <tdk/event_loop/scheduler.hpp>
#include <algorithm>
namespace tdk {

scheduler::scheduler( void ){

}

scheduler::~scheduler( void ){

}

void scheduler::schedule( tdk::timer_task* tt ) {
	std::list<tdk::timer_task*>::iterator it = 
        std::upper_bound( _timer_tasks.begin() , _timer_tasks.end() , tt ,
			[]( tdk::timer_task* v , tdk::timer_task* cmp )->bool{
                return v->expired_at() < cmp->expired_at();
            });
    _timer_tasks.insert( it , tt );
}

bool scheduler::cancel( tdk::timer_task* tt ) {
	auto it = std::find( _timer_tasks.begin() , _timer_tasks.end() , tt );
    if ( it != _timer_tasks.end() ) {
        _timer_tasks.erase( it );
		return true;
    }
	return false;
}
	
int scheduler::schedule( void ) {
	int cnt = 0;
	while( !_timer_tasks.empty()){
        tdk::timer_task* tt = _timer_tasks.front();
        if ( tt->expired_at() <= tdk::date_time::utc()){
            _timer_tasks.pop_front();
            (*tt)();
			++cnt;
        } else {
            break;
        }
    }
	return cnt;
}

void scheduler::schedule( tdk::slist_queue< tdk::timer_task >& expireds ) {
	while( !_timer_tasks.empty()){
        tdk::timer_task* tt = _timer_tasks.front();
        if ( tt->expired_at() <= tdk::date_time::utc()){
            _timer_tasks.pop_front();
			expireds.add_tail( tt );
        } else {
            break;
        }
    }
}

tdk::time_span scheduler::schedule_after( void ) {
	if ( _timer_tasks.empty())
		return tdk::time_span::from_seconds(100);
	tdk::date_time now = tdk::date_time::utc();
	tdk::date_time expired_at = (*_timer_tasks.begin())->expired_at();
	if ( now > expired_at )
		return tdk::time_span(0);
	return expired_at - now;
}

}
