#ifndef __tdk_task_timer_operation_h__
#define __tdk_task_timer_operation_h__

#include <tdk.task/task/operation.hpp>
#include <tdk/util/rc_ptr.hpp>
#include <tdk/time/date_time.hpp>
#include <vector>

namespace tdk {
namespace task{

class timer_operation 
	: public tdk::task::operation 
	, public tdk::rc_ptr_base< timer_operation >
{
public:
	timer_operation( void );
	virtual ~timer_operation( void );

	void expired_at( const tdk::date_time& at );
	const tdk::date_time& expired_at(void);
private:
	tdk::date_time _expired_at;
};


}}


#endif

