#include "stdafx.h"
#include <tdk.task/task/timer.hpp>
#include <tdk.task/task/event_loop.hpp>

namespace tdk {
namespace task {

timer::timer( event_loop& loop ) 
	: event_loop_object( loop )
{
}

timer::~timer( void ){

}

void timer::expired_at( const tdk::date_time& time ) {
	_expired_at = time;
}


void timer::schedule( void ) {
	_op->expired_at( _expired_at );
	loop().scheduler().schedule( _op );
}

void timer::cancel( void ) {
	loop().scheduler().cancel( _op );
}

}}