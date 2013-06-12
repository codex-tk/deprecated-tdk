#include "stdafx.h"
#include <tdk.task/task/event_loop_group.hpp>

namespace tdk {
namespace task {

event_loop_group::event_loop_group( void )
	:_idx(0)
{
}
event_loop_group::~event_loop_group( void ){
}

bool event_loop_group::open( int io_threads ) {
	for( int i = 0 ; i < io_threads ; ++i ) {
		_threads.push_back( new event_loop_thread() );
		if (!_threads.back()->open()) {
			return false;
		}
	}
	return true;
}

tdk::task::event_loop& event_loop_group::next_loop( void ) {
	tdk::threading::scoped_lock<> guard(_lock);
	tdk::task::event_loop& loop = _threads[_idx]->loop();
	++_idx;
	_idx %= _threads.size();
	return loop;
}

}}
