#include "stdafx.h"
#include <tdk/io/iocp.hpp>

#if defined ( _WIN32 )
namespace tdk {
namespace io {

iocp::iocp( void ) 
	: _iocp( CreateIoCompletionPort( INVALID_HANDLE_VALUE , NULL , NULL , 0 ))
{
}

iocp::~iocp( void ){
	if ( _iocp  != INVALID_HANDLE_VALUE ) {
        CloseHandle( _iocp );
    }
    _iocp = INVALID_HANDLE_VALUE;	
}


bool iocp::register_handle( SOCKET fd , void* k) {
	return CreateIoCompletionPort( reinterpret_cast< HANDLE >(fd) 
        , _iocp
        , reinterpret_cast< ULONG_PTR >( k )
        , 0 ) != NULL;
}

void iocp::unregister_handle( SOCKET fd ) {
	//
}

void iocp::wake_up( void ) {
	PostQueuedCompletionStatus( _iocp , 0 , 0 , 0 ); 
}

void iocp::wait( const tdk::time_span& w ) {
	LPOVERLAPPED overlapped = nullptr;
    ULONG_PTR key = 0;
    DWORD bytes_transferred = 0;
    
    bool result = GetQueuedCompletionStatus( 
        _iocp
        , &bytes_transferred 
        , &key
        , &overlapped
        , static_cast<DWORD>(w.total_milli_seconds())) == TRUE;

	if ( overlapped == nullptr )
		return;
	io::task* task_ptr = 
		static_cast< io::task::impl_type* >(overlapped)->data.ptr;
	if ( !result ) {
		task_ptr->error( tdk::platform::error());
	}
	task_ptr->io_bytes(bytes_transferred);
	(*task_ptr)();
}

}}
#endif
