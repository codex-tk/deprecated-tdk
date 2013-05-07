#ifndef __tdk_task_event_loop_thread_h__
#define __tdk_task_event_loop_thread_h__

#include <tdk/threading/thread.hpp>
#include <tdk.task/task/event_loop.hpp>

namespace tdk {
namespace task {

class event_loop_thread {
public:
	event_loop_thread ( void ) {
		_thread = new tdk::threading::thread( [this]()->int{
			handler();
			return 0;
		});
		_loop = new tdk::task::event_loop();
	}

	~event_loop_thread ( void ) {
	
	}

	bool open( void ) {
		if ( _loop->open() ) {
			return _thread->start();
		}
		return false;
	}

	void close( void ) {

	}

	void handler( void ){
		_loop->increment_ref();
		_loop->run();
	}

	tdk::task::event_loop& loop( void ) {
		return *_loop;
	}
private:
	tdk::threading::thread* _thread;
	tdk::task::event_loop* _loop;
};

}}

#endif