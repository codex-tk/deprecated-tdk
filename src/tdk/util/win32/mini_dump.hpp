#ifndef __tdk_diag_mini_dump_h__
#define __tdk_diag_mini_dump_h__

#include <DbgHelp.h>

namespace tdk {
namespace util {

class mini_dump{
public:
	mini_dump( MINIDUMP_TYPE type );
	~mini_dump(void);

	bool dump( const std::wstring& file_name 
			,  EXCEPTION_POINTERS *pExp );
private:
	typedef BOOL (WINAPI *pf_write_dump)(
	    HANDLE hProcess, 
        DWORD dwPid, 
        HANDLE hFile, 
        MINIDUMP_TYPE DumpType,
	    CONST PMINIDUMP_EXCEPTION_INFORMATION	ExceptionParam,
	    CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	    CONST PMINIDUMP_CALLBACK_INFORMATION	CallbackParam
	);
	pf_write_dump _write_dump;
	HMODULE		_dbg_help_dll;
	MINIDUMP_TYPE _type;
};

}}

#endif