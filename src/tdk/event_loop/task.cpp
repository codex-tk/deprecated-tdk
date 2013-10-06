#include "stdafx.h"
#include <tdk/event_loop/task.hpp>

namespace tdk {

task::task( void ) 
	: _handler( nullptr ) , _ctx( nullptr )
{
}

task::task( handler h , void* ctx ) 
	: _handler( h ) , _ctx( ctx )
{
}

task::~task( void ) {

}

void task::set_handler( handler h , void* ctx ) {
	_handler = h;
	_ctx = ctx;
}

void* task::context( void ) {
	return _ctx;
}

void task::operator()( void ) {
	if ( _handler )
		(*_handler)( this );
}

}