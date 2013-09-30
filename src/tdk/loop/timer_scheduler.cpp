#include <tdk/loop/timer_scheduler.hpp>
#include <tdk/error/error_platform.hpp>
#include <algorithm>

namespace tdk {

timer_scheduler::timer_scheduler( void ){

}

timer_scheduler::~timer_scheduler( void ) {

}

void timer_scheduler::schedule( tdk::timer_req* req ) {
    if ( _reqs.empty() ) {
        _reqs.push_back( req );
        return;
    }
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
        _reqs.erase( it );
    }
}

void timer_scheduler::drain( void ) {
    while( !_reqs.empty()){
        tdk::timer_req* r = _reqs.front();
        if ( r->expired_at() <= tdk::date_time::utc()){
            _reqs.pop_front();
            r->invoke();
        } else {
            return;
        }
    }
}

tdk::time_span timer_scheduler::wake_up_after( void ) {
    if (!_reqs.empty()){
        tdk::date_time now = tdk::date_time::utc();
        if ( now > _reqs.front()->expired_at()){
            return tdk::time_span::from_seconds(0); 
        }
        return _reqs.front()->expired_at() - now;
    }
    return tdk::time_span::from_seconds(1);
}

bool timer_scheduler::is_empty( void ) {
    return _reqs.empty();
}

}
