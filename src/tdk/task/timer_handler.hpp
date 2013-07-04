#ifndef __tdk_task__timer_handler_h__
#define __tdk_task__timer_handler_h__

#include <tdk/time/date_time.hpp>
#include <tdk/util/rc_ptr.hpp>

namespace tdk {
namespace task {

class timer_handler : public tdk::rc_ptr_base< timer_handler >{
public:
	timer_handler( void );
	virtual ~timer_handler( void );
	virtual void operator()( const std::error_code& e) = 0;
	void expired_at( const tdk::date_time& at );
	const tdk::date_time& expired_at(void);
private:
	tdk::date_time _expired_at;;
};
typedef tdk::rc_ptr< timer_handler > timer_id;

}}

#endif