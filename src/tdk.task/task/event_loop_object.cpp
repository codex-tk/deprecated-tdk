#include "stdafx.h"
#include <tdk.task/task/event_loop_object.hpp>


namespace tdk {
namespace task {

event_loop_object::event_loop_object( event_loop& loop )
	: _loop( &loop )
{
}

void event_loop_object::loop( event_loop* l ) {
	_loop = l;
}

event_loop& event_loop_object::loop( void ) {
	return *_loop;
}

event_loop_object::~event_loop_object( void ){

}


}}