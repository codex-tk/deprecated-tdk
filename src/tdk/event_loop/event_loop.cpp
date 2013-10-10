#include "stdafx.h"
#include <tdk/event_loop/event_loop.hpp>

namespace tdk {

event_loop::event_loop( void ) 
	: _active_handles(0)
	, _thread_id( std::this_thread::get_id())
{

}

event_loop::~event_loop( void ) {

}

void event_loop::execute( tdk::task* t ) {
	if ( in_loop()) {
		_tasks.add_tail( t );
	} else {
		tdk::threading::scoped_lock<> guard( _lock );
		_tasks_thread.add_tail( t );
	}
	_active_handles.fetch_add(1);
	_io_impl.wake_up();
}

bool event_loop::in_loop( void ) {
	return std::this_thread::get_id() == _thread_id;
}

void event_loop::schedule( tdk::timer_task* tt ) {
	_scheduler.schedule( tt );
	_active_handles.fetch_add(1);
}

bool event_loop::cancel( tdk::timer_task* tt ) {
	if ( _scheduler.cancel( tt ) ){
		_active_handles.fetch_sub(1);
		return true;
	}
	return false;
}

void event_loop::run( void ) {
	_thread_id = std::this_thread::get_id();
	while ( _active_handles.load() ) {
		_io_impl.wait(_scheduler.schedule_after());
		int cnt = _scheduler.schedule();
		cnt += _run_tasks();
		_active_handles.fetch_sub( cnt );
	}
}

int event_loop::_run_tasks( void ) {
	int cnt = 0;
	do {
		tdk::threading::scoped_lock<> guard( _lock );
		_tasks.add_tail( _tasks_thread );
	}while(0);
	while ( !_tasks.is_empty()){
		tdk::task* t = _tasks.front();
		_tasks.pop_front();
		(*t)();
		++cnt;
	}
	return cnt;
}

void event_loop::add_active( void ) {
	_active_handles.fetch_add(1);
}

void event_loop::remove_active( void ) {
	_active_handles.fetch_sub( 1 );
}

tdk::io::epoll& event_loop::io_impl( void ) {
	return _io_impl;
}

event_loop& event_loop::default_loop( void ) {
	static event_loop l;
	return l;
}

}
