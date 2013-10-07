#ifndef __tdk_event_loop_h__
#define __tdk_event_loop_h__

#include <atomic>

#if defined ( linux ) || defined( __linux )
#include <tdk/event_loop/io/epoll.hpp>
#elif defined( _WIN32 )
#include <tdk/event_loop/io/iocp.hpp>
#endif

#include <tdk/event_loop/scheduler.hpp>
#include <tdk/threading/spin_lock.hpp>

#include <thread>
#include <list>

namespace tdk {

class event_loop{
public:
	event_loop( void );
	~event_loop( void );
	
	// thread 처리
	void execute( tdk::task* t );

	void run( void );
	
	bool in_loop( void );

	// loop 쓰레드 내에서만 호출
	void schedule( tdk::timer_task* tt );
	bool cancel( tdk::timer_task* tt );
private:
	int _run_tasks( void );
private:
	tdk::threading::spin_lock _lock;
	std::atomic<int> _active_handles;    
	tdk::scheduler _scheduler;
	tdk::slist_queue< tdk::task > _tasks;
	tdk::slist_queue< tdk::task > _tasks_thread;
	std::thread::id _thread_id;
};

}
#endif