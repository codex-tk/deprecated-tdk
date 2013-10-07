#include "stdafx.h"
#include <tdk/event_loop/timer_task.hpp>


namespace tdk {

timer_task::timer_task( void ) 
	: _expired_at( tdk::date_time::utc())
{

}

timer_task::timer_task( task::handler h , void* ctx )
	: task( h , ctx ) 
	, _expired_at( tdk::date_time::utc())
{

}

timer_task::~timer_task( void ) {

}

const std::error_code& timer_task::error( void ) const {
	return _error;
}

void timer_task::error( const std::error_code& ec ) {
	_error = ec;
}

void timer_task::expried_at( const tdk::date_time& dt ) {
	_expired_at = dt;
}

const tdk::date_time& timer_task::expired_at( void ) const {
	return _expired_at;
}

}
