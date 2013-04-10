#include "stdafx.h"

#include <tdk/tdk.hpp>
#include <tdk/error/last_error.hpp>

#include <tdk.task/task/win32/io_completion_port.hpp>
#include <tdk.task/task/win32/io_engine.hpp>

namespace tdk {
namespace task {
io_completion_port::io_completion_port(io_engine& engine) 
	: _handle( INVALID_HANDLE_VALUE )
	, _engine( engine )
{

}

io_completion_port::~io_completion_port( void ) {
	close();
}

bool io_completion_port::open( void ) {
	close();

	_handle = CreateIoCompletionPort( INVALID_HANDLE_VALUE , NULL , NULL , 0 );
	if ( _handle == INVALID_HANDLE_VALUE ) {
		TDK_ASSERT( !"CreateIoCompletionPort Failed" );
		tdk::set_last_error( tdk::platform_error());
		return false;
	}
	return true;
}

void io_completion_port::close( void ) {
	if ( _handle  != INVALID_HANDLE_VALUE ) {
		CloseHandle( _handle );
	}
	_handle = INVALID_HANDLE_VALUE;	
}

bool io_completion_port::bind( SOCKET fd , void* object ) {
	if ( _handle == NULL ) {
		TDK_ASSERT( !"uninitialized port" );
		tdk::set_last_error( tdk::platform_error());
		return false;
	}

	if ( fd == INVALID_SOCKET ) {
		TDK_ASSERT( !"invalid_parameter" );
		tdk::set_last_error( tdk::platform_error());
		return false;
	}
	if ( CreateIoCompletionPort( reinterpret_cast< HANDLE >(fd) 
		, _handle
		, reinterpret_cast< ULONG_PTR >( object )
		, 0 ) == NULL ) 
	{
		TDK_ASSERT( !"CreateIoCompletionPort Failed" );
		tdk::set_last_error( tdk::platform_error());
		return false;
	}
	return true;
}

void io_completion_port::unbind( SOCKET ) {

}

HANDLE io_completion_port::handle(void)const {
	return _handle;
}

bool io_completion_port::post( int byte , void* key , OVERLAPPED* ov  ) {
	if ( _handle == NULL ) {
		TDK_ASSERT( !"uninitialized port" );
		tdk::set_last_error( tdk::platform_error());
		return false;
	}

	if ( PostQueuedCompletionStatus( 
				handle()
				, byte
				, reinterpret_cast< ULONG_PTR>( key )
				, ov ) == FALSE )
	{
		TDK_ASSERT( !"PostQueuedCompletionStatus Failed" );
		tdk::set_last_error( tdk::platform_error());
		return false;
	}
	return true;
}

int io_completion_port::wait( const tdk::time_span& t ){
#ifdef COMPLETION_PORT_USE_GQCS_EX
	OVERLAPPED_ENTRY entry[64];
	ULONG entry_count;

	bool result = GetQueuedCompletionStatusEx( handle()
						, entry
						, 64
						, &entry_count
						, static_cast<DWORD>(t.total_milli_seconds())
						, FALSE ) == TRUE;

	if ( !result ) {
		tdk::error_code code = tdk::platform_error();
		switch ( code.value() ) {
		case WAIT_TIMEOUT:
			break;
		case ERROR_ABANDONED_WAIT_0:
			break;
		}
		tdk::set_last_error( code );
		return 0;
	}

	for ( ULONG i = 0 ; i < entry_count ; ++i ) {
		tdk::error_code code = tdk::platform_error( static_cast<int>(entry[i].Internal));
		_engine.on_complete( code 
			, entry[i].dwNumberOfBytesTransferred
			, reinterpret_cast< void* >(entry[i].lpCompletionKey)
			, entry[i].lpOverlapped );
	}
	return entry_count;
#else	
	LPOVERLAPPED overlapped = nullptr;
	ULONG_PTR key = 0;
	DWORD bytes_transferred = 0;
    
	bool result = GetQueuedCompletionStatus( handle() 
		, &bytes_transferred 
		, &key
		, &overlapped
		, static_cast<DWORD>(t.total_milli_seconds())) == TRUE;

	if ( !result ) {
		tdk::error_code code = tdk::platform_error();
		switch ( code.value() ) {
		case WAIT_TIMEOUT:
			break;
		case ERROR_ABANDONED_WAIT_0:
			break;
		}
		tdk::set_last_error( code );
		return 0;
	}
	_engine.on_complete( tdk::platform_error() 
			, bytes_transferred
			, reinterpret_cast< void* >(key)
			, overlapped );

	return 1;
#endif
}

}}