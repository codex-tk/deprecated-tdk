#ifndef __tdk_task_scheduler_h__
#define __tdk_task_scheduler_h__

#include <list>
#include <tdk/io/engine_win32.hpp>
#include <tdk/threading/spin_lock.hpp>

namespace tdk {

namespace task {
		
class timer_scheduler {
public:
	explicit timer_scheduler( tdk::io::engine* dispatcher = nullptr ) ;
	~timer_scheduler( void );

	//void schedule( const timer_id& id );
	//bool cancel( const timer_id& id );
	//tdk::time_span next_schedule(void);

	//void drain( void );
private:

private:
	tdk::threading::spin_lock _lock;
	tdk::io::engine* _engine;
	//std::list< timer_id > _timers;
	//std::list< timer_id > _cancelled;
};

}}

#endif