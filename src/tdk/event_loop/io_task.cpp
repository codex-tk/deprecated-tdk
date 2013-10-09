#include "stdafx.h"
#include <tdk/event_loop/io_task.hpp>

namespace tdk {

io_task::io_task( void )
	:_error( std::error_code())
	, _io_bytes(0)
{

}

io_task::io_task( task::handler h , void* ctx )
	: task( h , ctx )
	, _error( std::error_code())
	, _io_bytes(0){

}

io_task::~io_task( void ) {

}

const std::error_code& io_task::error( void ) {
	return _error;
}

void io_task::error( const std::error_code& ec ) {
	_error = ec;
}

int io_task::io_bytes( void ) {
	return _io_bytes;
}

void io_task::io_bytes( int io ) {
	_io_bytes = io;
}

}
