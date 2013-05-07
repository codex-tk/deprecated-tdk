#ifndef __tdk_task_event_loop_group_h__
#define __tdk_task_event_loop_group_h__

#include <tdk.task/task/event_loop_thread.hpp>

namespace tdk {
namespace task {
	
class event_loop_group{
public:
	event_loop_group( void );
	~event_loop_group( void );

	bool open( int io_threads );

	tdk::task::event_loop& next_loop( void ) ;
private:
	tdk::threading::spin_lock _lock;
	std::vector< event_loop_thread* > _threads;
	int _idx;
};

}}

#endif