#include <tdk/loop/loop.hpp>
#include <sys/epoll.h>
#include <tdk/threading/thread_local.hpp>
#include <unistd.h>

namespace tdk {
namespace detail {

static tdk::threading::thread_local< tdk::loop* >& 
    current_loop( void ) {
    static tdk::threading::thread_local< tdk::loop* > _instance;
    return _instance;
}
   
}

loop::loop( void ){
    _active_handles.store(0);
}

loop::~loop( void ){
}

loop& loop::default_loop( void ) {
    static loop _loop;
    return _loop;
}

loop* loop::current_loop( void ) {
    return detail::current_loop().get();
}

loop::impl_type& loop::implementation( void ) {
    return _impl;
}

tdk::timer_scheduler& loop::scheduler( void ) {
    return _scheduler;
}

void loop::add_active( void ) {
    _active_handles.fetch_add(1);
}

void loop::remove_active( void ) {
    _active_handles.fetch_sub(1);
}

void loop::run( void ) {
    detail::current_loop().set( this );
    while ( _active_handles.load()){
        _impl.wait( _scheduler.wake_up_after());
        _run_reqs();
    }
    detail::current_loop().set( nullptr );
}

void loop::execute( tdk::req_handle* req ) {
    add_active();
    tdk::threading::scoped_lock<> guard( _lock );
    _req_queue.add_tail( req );
    if ( current_loop() == this ) {
        return;
    }
    implementation().wake_up();
}

void loop::_run_reqs( void ) {
    tdk::slist_queue< tdk::req_handle > reqs;
    do {
        tdk::threading::scoped_lock<> guard( _lock );
        reqs.add_tail( _req_queue  );    
    }while(0);
    while ( !reqs.is_empty() ) {
        tdk::req_handle* req = reqs.front();
        reqs.pop_front();
        req->invoke();
        remove_active();
    }
}

}
