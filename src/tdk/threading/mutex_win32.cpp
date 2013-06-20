#include "stdafx.h"
#include <tdk/threading/mutex_win32.hpp>

namespace tdk {
namespace threading {

mutex::mutex( void )
	: _mutex(INVALID_HANDLE_VALUE){
}

mutex::~mutex( void ) {
	close();
}

bool mutex::create( const  wchar_t* name , bool initstate ) {
	_mutex = CreateMutex( NULL , initstate ? TRUE : FALSE , name );
	if ( _mutex == NULL ) {
		tdk::set_last_error( tdk::platform::error());
		return false;
	}
	return true;
}

bool mutex::open( const tdk::tstring& name ) {
	_mutex = OpenMutex( MUTEX_ALL_ACCESS, FALSE, name.c_str() );
	if ( _mutex == NULL ) {
		tdk::set_last_error( tdk::platform::error());
		return false;
	}
	return true;
}

void mutex::close( void ) {
	if ( _mutex != INVALID_HANDLE_VALUE ) {
		CloseHandle( _mutex );
	}
	_mutex = INVALID_HANDLE_VALUE;
}


bool mutex::wait( const tdk::time_span& ts ) {
	if ( _mutex == INVALID_HANDLE_VALUE ) {
		tdk::set_last_error( tdk::platform::error( ERROR_INVALID_HANDLE ));
        return false;
    }
	int ret = ::WaitForSingleObject( _mutex , 
		static_cast< uint32_t >( ts.total_milli_seconds()));

	switch( ret ) {
    case WAIT_OBJECT_0:
		return true;
	case WAIT_FAILED:
        tdk::set_last_error( tdk::platform::error());
		return false;
	}
	tdk::set_last_error( tdk::platform::error(ret));
	return false;
}

void mutex::release( void ) {
	ReleaseMutex( _mutex );
}

}}
