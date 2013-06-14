#ifndef __tdk_task_queue_timer_h__
#define __tdk_task_queue_timer_h__

#include <tdk/threading/spin_lock.hpp>
#include <tdk/time/date_time.hpp>
#include <tdk/util/rc_ptr.hpp>
#include <list>

namespace tdk {
namespace task {

class queue_timer {
public:
	class handler : public tdk::rc_ptr_base< handler >{
	public:
		handler( void ){}
		virtual ~handler( void ){}
		virtual void operator()( const tdk::error_code& e) = 0;

		void expired_at( const tdk::date_time& at );
		const tdk::date_time& expired_at(void);
	private:
		tdk::date_time _expired_at;;
	};
	typedef tdk::rc_ptr< handler > timer_id;
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