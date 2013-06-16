#ifndef __tdk_task_queue_timer_h__
#define __tdk_task_queue_timer_h__

#include <tdk/threading/spin_lock.hpp>
#include <tdk/task/timer_handler.hpp>
#include <list>

namespace tdk {
namespace task {

class queue_timer {	
public:
	queue_timer( void );
	~queue_timer(void);

	void schedule( const timer_id& id );
	bool cancel( const timer_id& id );
	
	static queue_timer& instance( void );

	void on_timer( void );
	static void __stdcall _on_timer( void* p , BOOLEAN timer_or_wait_fired ) ;

	void drain_cancels( void );
	void drain_expired( void );
private:
	HANDLE _timer;
	tdk::threading::spin_lock _lock;
	std::list< timer_id > _timers;
	std::list< timer_id > _cancels;
};


}}


#endif