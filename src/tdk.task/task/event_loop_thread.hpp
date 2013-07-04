#ifndef __tdk_task_event_loop_thread_h__
#define __tdk_task_event_loop_thread_h__

//#include <tdk/threading/thread.hpp>
#include <tdk.task/task/event_loop.hpp>
#include <thread>

namespace tdk {
namespace task {

class event_loop_thread {
public:
	event_loop_thread ( void ) ;
	~event_loop_thread ( void );
	bool open( void );
	void close( void );
	void handler( void );

	tdk::task::event_loop& loop( void ) ;
private:
	std::thread _thread;
	//tdk::threading::thread* _thread;
	tdk::task::event_loop* _loop;
};

}}

#endif