#include "stdafx.h"
#include <tdk/threading/thread.hpp>
#include <tdk/threading/thread_data.hpp>
#include <tdk/error/last_error.hpp>
#include <tdk/error/platform_error.hpp>
/*
#include "last_error.hpp"
#include "mini_dump_handler.hpp"
#include "lib_tls.hpp"
*/
namespace tdk { namespace threading {

unsigned int __stdcall thread_entry_point( void* arg ) {
	unsigned int ret = 0;
	tdk::threading::data data;
	tdk::threading::data::set_thread_data( &data );
	if ( arg ) {
		thread* thr = static_cast< thread* >( arg );
		ret = thr->run();
	}
	return ret;
}

thread::~thread( void ) {
	stop( tdk::time_span::infinite() );
}

bool thread::start( void ) {
	unsigned int tid = 0;
	_handle = reinterpret_cast < HANDLE >(
		_beginthreadex(
			nullptr 
			, 0 
			, &thread_entry_point 
			, this 
			, 0 
			, &tid ));
	tdk::set_last_error( tdk::platform_error() );
	return _handle != INVALID_HANDLE_VALUE;
}

bool thread::stop( const tdk::time_span& wait ) {
	if ( _handle != INVALID_HANDLE_VALUE ) {
		switch( WaitForSingleObject( _handle 
			, static_cast< DWORD >(wait.total_micro_seconds())) ) {
		case WAIT_OBJECT_0:
			CloseHandle( _handle );
			_handle = INVALID_HANDLE_VALUE;
			break;
		case WAIT_FAILED:
			break;
		}
		tdk::set_last_error( tdk::platform_error() );
	}
	return _handle == INVALID_HANDLE_VALUE;
}

int thread::run( void ) {
	return _handler();
}


}}