#ifndef __tdk_timer_task_h__
#define __tdk_timer_task_h__

#include <system_error>
#include <tdk/event_loop/task.hpp>
#include <tdk/time/date_time.hpp>

namespace tdk {

class timer_task : public task {
public:
	timer_task( void );
	timer_task( task::handler h , void* ctx = nullptr );

	~timer_task( void );

	const std::error_code& error( void ) const;
	void error( const std::error_code& ec ) ;

	void expired_at( const tdk::date_time& dt );
	const tdk::date_time& expired_at( void ) const;
private:
	std::error_code _error;
	tdk::date_time _expired_at;
};

}


#endif
