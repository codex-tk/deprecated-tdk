#include "stdafx.h"
#include <tdk/util/win32/mini_dump.hpp>

namespace tdk {
namespace util {

	
mini_dump::mini_dump( MINIDUMP_TYPE type )
	: _type(type) , _dbg_help_dll( LoadLibrary( _T("DBGHELP.DLL") ) ) 
	, _write_dump( nullptr )
{
	if ( _dbg_help_dll != nullptr ) {
		_write_dump = reinterpret_cast< pf_write_dump  >( 
									GetProcAddress( _dbg_help_dll , "MiniDumpWriteDump" ));
	}
}


mini_dump::~mini_dump(void)
{

}

bool mini_dump::dump( const std::wstring& file_name 
			,  EXCEPTION_POINTERS *pExp)
{
	
	if ( _write_dump == nullptr ) {
//		err = caffe::diag::error( caffe::diag::caffe_category::errorcode::CAFFE_LOAD_MODULE_FAIL ,
//			caffe::diag::error::caffe_category());
		return false;
	}

	HANDLE file = CreateFileW(  file_name.c_str() , 
			                    GENERIC_READ | GENERIC_WRITE, 
                                0, 
                                NULL, 
                                CREATE_ALWAYS, 
                                FILE_ATTRIBUTE_NORMAL, 
                                NULL );

    if ( file == INVALID_HANDLE_VALUE ) {
		//err = caffe::diag::error::system_error();
        return false;
	}
    
    MINIDUMP_EXCEPTION_INFORMATION ExInfo;

	ExInfo.ThreadId           = GetCurrentThreadId(); 
	ExInfo.ExceptionPointers  = pExp; 
	ExInfo.ClientPointers     = FALSE; 

	BOOL ret = _write_dump(GetCurrentProcess(), 
				GetCurrentProcessId(), 
				file, 
				_type , 
				&ExInfo , 
				NULL , 
				NULL );

    CloseHandle( file );
	//err = caffe::diag::error::system_error();
	return ret == TRUE;
}

}}
