#include "stdafx.h"
#include <tdk/task/timer_handler.hpp>

namespace tdk {
namespace task {

timer_handler::timer_handler( void ){
}

timer_handler::~timer_handler( void ){
}

void timer_handler::expired_at( const tdk::date_time& at ) {
	_expired_at = at;
}

const tdk::date_time& timer_handler::expired_at(void) {
	return _expired_at;
}

}}
