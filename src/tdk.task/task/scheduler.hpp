#ifndef __tdk_task_scheduler_h__
#define __tdk_task_scheduler_h__

#include <list>
#include <tdk.task/task/operation/timer_operation.hpp>
#include <tdk/threading/spin_lock.hpp>

namespace tdk {
namespace task {
class event_loop;
// 다른 쓰레드에서 등록 가능
class scheduler {
public:
	typedef tdk::rc_ptr< tdk::task::timer_operation> timer_id;
public:
	scheduler( event_loop& loop );
	~scheduler( void );

	void schedule( const timer_id& id );
	bool cancel( const timer_id& id );
	tdk::time_span next_schedule(void);

	void drain( void );
private:
	event_loop& _loop;
	tdk::threading::spin_lock _lock;
	std::list< timer_id > _timers;
	std::list< timer_id > _cancelled;
};

}}

#endif