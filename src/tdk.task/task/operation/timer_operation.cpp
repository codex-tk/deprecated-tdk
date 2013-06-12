#include "stdafx.h"
#include <tdk.task/task/operation/timer_operation.hpp>

namespace tdk {
namespace task{
timer_operation::timer_operation(void )
{
}

timer_operation::~timer_operation( void ) {

}

void timer_operation::expired_at( const tdk::date_time& at ) {
	_expired_at = at;
}

const tdk::date_time& timer_operation::expired_at(void) {
	return _expired_at;
}
}}
