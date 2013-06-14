#include "stdafx.h"
#include <tdk/util/win32/mini_dump_seh_handler.hpp>
#include <tdk/time/date_time.hpp>
#include <tdk/threading/thread.hpp>

namespace tdk {
namespace util {

mini_dump_seh_handler::mini_dump_seh_handler( MINIDUMP_TYPE type )
	: _dump( type ) {

}

mini_dump_seh_handler::~mini_dump_seh_handler(void) {

}

void mini_dump_seh_handler::handle_exception( EXCEPTION_POINTERS *pExp ) {
	if ( pExp->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW ) {
		tdk::threading::thread thread( [ this,pExp](){
			_write_dump(pExp);
			return 0;
		});
		thread.start();
		thread.stop( tdk::time_span::infinite() );
	} else {
		_write_dump( pExp );
	}
}


void mini_dump_seh_handler::_write_dump( EXCEPTION_POINTERS* pExp ) {
	wchar_t path[MAX_PATH];

	GetModuleFileNameW( NULL, path , MAX_PATH * sizeof(wchar_t)); 
    wchar_t *ext = wcsrchr( path , '.' );

	SYSTEMTIME st = tdk::time::tick::to_systemtime(tdk::date_time::local().time());

    wsprintf( ext ? ext : path + wcslen( path ) , 
				L"_%04d%02d%02d_%02d%02d%02d_ProcessId_%d.dmp" , 
				st.wYear , st.wMonth , st.wDay ,
				st.wHour , st.wMinute, st.wMinute ,
				GetCurrentProcessId());

	_dump.dump( std::wstring( path ) , pExp );
}

}}
