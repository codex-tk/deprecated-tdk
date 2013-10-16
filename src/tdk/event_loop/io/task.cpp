/*
 * task.cpp
 *
 *  Created on: 2013. 10. 14.
 *      Author: tk
 */
#include "stdafx.h"
#include <tdk/event_loop/io/task.hpp>

namespace tdk {
namespace io {

task::task( void )
	:_error( std::error_code())
	, _io_bytes(0)
{

}

task::task( tdk::task::handler h , void* ctx )
	: tdk::task( h , ctx )
	, _error( std::error_code())
	, _io_bytes(0){

}

task::~task( void ) {

}

const std::error_code& task::error( void ) {
	return _error;
}

void task::error( const std::error_code& ec ) {
	_error = ec;
}

int task::io_bytes( void ) {
	return _io_bytes;
}

void task::io_bytes( int io ) {
	_io_bytes = io;
}


} /* namespace io */
} /* namespace tdk */
