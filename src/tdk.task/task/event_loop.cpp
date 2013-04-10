#include "stdafx.h"
#include <tdk.task/task/event_loop.hpp>

namespace tdk {
namespace task {

event_loop::event_loop()
	: _engine( *this )
{

}

event_loop::~event_loop(){

};


io_engine& event_loop::engine( void ){ 
	return _engine;
}

bool event_loop::run( const tdk::time_span& wait ) {
	return false;
}

}}
