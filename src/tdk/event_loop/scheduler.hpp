#ifndef __tdk_event_loop_scheduler_h__
#define __tdk_event_loop_scheduler_h__

#include <list>
#include <tdk/event_loop/timer_task.hpp>
#include <tdk/util/list_node.hpp>

namespace tdk {

class scheduler{
public:
    scheduler( void );
    ~scheduler( void );
	
	void schedule( tdk::timer_task* tt );
	bool cancel( tdk::timer_task* tt );	
	
	int schedule( void );

	void schedule( tdk::slist_queue< tdk::timer_task >& expireds );

	tdk::time_span schedule_after( void );
private:
    std::list< tdk::timer_task* > _timer_tasks;
};

}

#endif
