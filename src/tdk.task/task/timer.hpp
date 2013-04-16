#ifndef __tdk_task_timer_h__
#define __tdk_task_timer_h__

#include <tdk.task/task/event_loop_object.hpp>
#include <tdk/time/date_time.hpp>
#include <tdk.task/task/operation/timer_operation.hpp>
namespace tdk {
namespace task {

class timer : public event_loop_object {
public:
	timer( event_loop& loop );
	~timer( void );

	void expired_at( const tdk::date_time& time );
	template <typename T_handler>
	void handler( const T_handler& handler ) {
		class timer_operation_impl : public timer_operation {
		public:
			timer_operation_impl( const T_handler& handler  )
				:_handler( handler )
			{
			}
			virtual ~timer_operation_impl( void ){
			}

			virtual void operator()( void ) {
				_handler( error());
			}
		private:
			T_handler _handler;
		};
		_op = new timer_operation_impl(handler);
	}

	void schedule( void );
	void cancel( void );
private:
	tdk::rc_ptr< tdk::task::timer_operation > _op;
	tdk::date_time _expired_at;
};

}}
#endif