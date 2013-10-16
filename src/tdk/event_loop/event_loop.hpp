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
	
	// thread safe
	void execute( tdk::task* t );

	void run( void );
	
	bool in_loop( void );

	void schedule( tdk::timer_task* tt );
	void cancel( tdk::timer_task* tt );

	void add_active( void );
	void remove_active( void );

	tdk::io::epoll& io_impl( void );

	//
	static event_loop& default_loop( void );
private:
	int _run_tasks( void );
private:
	tdk::threading::spin_lock _lock;
	std::atomic<int> _active_handles;    
	tdk::scheduler _scheduler;
	tdk::slist_queue< tdk::task > _tasks;
	tdk::slist_queue< tdk::task > _tasks_thread;
	std::thread::id _thread_id;
#if defined( linux ) || defined( __linux__ )
	tdk::io::epoll _io_impl;
#endif

};

}
#endif
