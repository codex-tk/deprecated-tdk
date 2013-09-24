#include <tdk/loop/timer_scheduler.hpp>
#include <tdk/error/error_platform.hpp>

#include <algorithm>

namespace tdk {

timer_scheduler::timer_scheduler( void ){

}

timer_scheduler::~timer_scheduler( void ) {

}

void timer_scheduler::schedule( tdk::timer_req* req ) {
    std::list<tdk::timer_req*>::iterator it = 
        std::upper_bound( _reqs.begin() , _reqs.end() , req ,
            []( tdk::timer_req* v , tdk::timer_req* cmp )->bool{
                return v->expired_at() < cmp->expired_at();
            });
    _reqs.insert( it , req );
}

void timer_scheduler::cancel( tdk::timer_req* req ) {
    auto it = std::find( _reqs.begin() , _reqs.end() , req );
    if ( it != _reqs.end() ) {
        _cancels.push_back( *it );
        _reqs.erase( it );
    }
}

void timer_scheduler::drain( void ) {
    while ( !_cancels.empty()){
        tdk::timer_req* r = _cancels.front();
        _cancels.pop_front();
        r->error( tdk::platform::error( ECANCELED ));
        r->invoke();
    }
    while( !_reqs.empty()){
        tdk::timer_req* r = _reqs.front();
        if ( r->expired_at() >= tdk::date_time::utc()){
            _reqs.pop_front();
            r->invoke();
        }
    }
}

tdk::date_time timer_scheduler::wakeup_at( void ) {
    if (!_cancels.empty()){
        return tdk::date_time::utc();
    }
    if (!_reqs.empty()){
        return _reqs.front()->expired_at();
    }
    return tdk::date_time::utc() + tdk::time_span::from_seconds(100);
}

}
