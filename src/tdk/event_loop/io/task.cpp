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
#if defined (_WIN32 )
#else
	:_error( std::error_code())
	, _io_bytes(0)
#endif
{
#if defined (_WIN32 )
	memset( &_impl , 0 , sizeof(_impl));
	_impl.task_ptr = this;
#endif
}

task::task( tdk::task::handler h , void* ctx )
	: tdk::task( h , ctx )
#if defined (_WIN32 )
#else
	, _error( std::error_code())
	, _io_bytes(0)
#endif
{
#if defined (_WIN32 )
	memset( &_impl , 0 , sizeof(_impl));
	_impl.task_ptr = this;
#endif
}

task::~task( void ) {

}

std::error_code task::error( void ) {
#if defined (_WIN32 )
	return std::error_code( static_cast<int>(_impl.Internal) , *(std::error_category*)_impl.InternalHigh);
#else
	return _error;
#endif
}

void task::error( const std::error_code& ec ) {
#if defined (_WIN32 )
	_impl.Internal = ec.value();
	_impl.InternalHigh = reinterpret_cast< ULONG_PTR >(&ec.category());
#else
	_error = ec;
#endif
}

int task::io_bytes( void ) {
#if defined (_WIN32 )
	return _impl.Offset;
#else
	return _io_bytes;
#endif
}

void task::io_bytes( int io ) {
#if defined (_WIN32 )
	_impl.Offset = io;
#else
	_io_bytes = io;
#endif
}

#if defined (_WIN32 )
OVERLAPPED* task::impl( void ) {
	return &_impl;
}

void task::reset( void ) {
	memset( &_impl , 0 , sizeof(_impl));
	_impl.task_ptr = this;
}
#endif


} /* namespace io */
} /* namespace tdk */
