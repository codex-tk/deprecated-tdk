#include "stdafx.h"
#include <tdk/io/completion_port_win32.hpp>

namespace tdk {
namespace io {
    
completion_port::completion_port(void){

}

completion_port::~completion_port(void){
}

bool completion_port::create( int concurrent_threads ){
    close();
    _handle = CreateIoCompletionPort( INVALID_HANDLE_VALUE , NULL , NULL , concurrent_threads );
    if ( _handle == INVALID_HANDLE_VALUE ) {
        TDK_ASSERT( !"CreateIoCompletionPort Failed" );
        tdk::set_last_error( tdk::platform::error());
        return false;
    }
    return true;
}

void completion_port::close( void ){
    if ( _handle  != INVALID_HANDLE_VALUE ) {
        CloseHandle( _handle );
    }
    _handle = INVALID_HANDLE_VALUE;	
}

int completion_port::wait( const tdk::time_span& w 
    , completion_port::callback cb 
    , void* ctx  )
{
    TDK_ASSERT( cb != nullptr && "[tdk::io::completion_port] callback is nullptr" );
    LPOVERLAPPED overlapped = nullptr;
    ULONG_PTR key = 0;
    DWORD bytes_transferred = 0;
    
    bool result = GetQueuedCompletionStatus( 
        _handle
        , &bytes_transferred 
        , &key
        , &overlapped
        , static_cast<DWORD>(w.total_milli_seconds())) == TRUE;

    tdk::error_code ec;
    if ( !result ) {
		ec = tdk::platform::error();
        switch ( ec.value() ) {
        case WAIT_TIMEOUT:
            return 0;
        case ERROR_ABANDONED_WAIT_0:
            tdk::set_last_error( ec );
            return -1;
        }
    }
    cb( ec  , bytes_transferred  , (void*)key  , overlapped  , ctx );
    return 1;
}


bool completion_port::bind( SOCKET fd , void* key ) {
    if ( _handle == NULL ) {
        TDK_ASSERT( !"uninitialized port" );
        tdk::set_last_error( tdk::platform::error());
        return false;
    }

    if ( fd == INVALID_SOCKET ) {
        TDK_ASSERT( !"invalid_parameter" );
        tdk::set_last_error( tdk::platform::error());
        return false;
    }
    if ( CreateIoCompletionPort( reinterpret_cast< HANDLE >(fd) 
        , _handle
        , reinterpret_cast< ULONG_PTR >( key )
        , 0 ) == NULL ) 
    {
        TDK_ASSERT( !"CreateIoCompletionPort Failed" );
        tdk::set_last_error( tdk::platform::error());
        return false;
    }
    return true;
}

bool completion_port::post( int io_byte , void* key , OVERLAPPED* ov ) {
    if ( _handle == NULL ) {
        TDK_ASSERT( !"uninitialized port" );
        tdk::set_last_error( tdk::platform::error());
        return false;
    }

    if ( PostQueuedCompletionStatus( 
                _handle
                , io_byte
                , reinterpret_cast< ULONG_PTR>( key )
                , ov ) == FALSE )
    {
        TDK_ASSERT( !"PostQueuedCompletionStatus Failed" );
        tdk::set_last_error( tdk::platform::error());
        return false;
    }
    return true;
}



}}