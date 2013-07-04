#include "stdafx.h"
#include <tdk.task/task/event_loop_thread.hpp>

namespace tdk {
namespace task {
	
event_loop_thread::event_loop_thread ( void ) {
	/*
	_thread = new tdk::threading::thread( [this]()->int{
		handler();
		return 0;
	});*/
	_loop = new tdk::task::event_loop();
}

event_loop_thread::~event_loop_thread ( void ) {
	delete _loop;
	//delete _thread;
}

bool event_loop_thread::open( void ) {
	if ( _loop->open() ) {
		_thread = std::thread([this](){ handler(); }); 
	}
	return false;
}

void event_loop_thread::close( void ) {
	_loop->decrement_ref();
	_loop->post([]{});
	_thread.join();
}

void event_loop_thread::handler( void ){
	_loop->increment_ref();
	_loop->run();
}

tdk::task::event_loop& event_loop_thread::loop( void ) {
	return *_loop;
}


}}