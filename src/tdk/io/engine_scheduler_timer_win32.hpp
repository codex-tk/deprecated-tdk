#ifndef __tdk_io_scheduler_tiemr_h__
#define __tdk_io_scheduler_tiemr_h__

#include <tdk/threading/spin_lock.hpp>
#include <tdk/io/engine_win32.hpp>
#include <set>

namespace tdk {
namespace io {

class engine::scheduler_timer {
public:
	scheduler_timer( void );
	~scheduler_timer( void );

	void reg( engine::scheduler* sch );
	void unreg( engine::scheduler* sch );

	void tick( void );
	static void __stdcall on_timer( void* p , BOOLEAN timer_or_wait_fired );
private:
	HANDLE _timer;
	tdk::threading::spin_lock _lock;
	std::set< engine::scheduler* > _scheduler;
};

}}


#endif