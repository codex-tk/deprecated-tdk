#ifndef __tdk_io_engine_scheduler_h__
#define __tdk_io_engine_scheduler_h__

#include <tdk/io/engine_win32.hpp>
#include <tdk/task/timer_handler.hpp>
#include <tdk/task/queue_timer_win32.hpp>
#include <list>

namespace tdk {
namespace io {

class engine::scheduler : public tdk::io::operation {
public:
	scheduler( tdk::io::engine& en );
	virtual ~scheduler( void );
	
	void open( void );
	void close( void );

	void schedule( tdk::io::engine::timer_id& id );
	bool cancel( tdk::io::engine::timer_id& id );

	void post_fail( operation* op );
	void drain( void );

	void drain_post_fails( void );
	void drain_cancels( void );
	void drain_expired( void );

	static void __stdcall _on_complete( operation* op );

	void on_timer( void );

	const tdk::date_time& schedule_time( void ) const ;

private:
	tdk::io::engine& _engine;
	mutable tdk::threading::spin_lock _lock;
	std::list< tdk::io::engine::timer_id > _timers;
	std::list< tdk::io::engine::timer_id > _cancels;
	tdk::slist_queue< tdk::io::operation > _op_queue;
	
	bool _in_progress;

	tdk::date_time _schedule_time;
};

}}


#endif