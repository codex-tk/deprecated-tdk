#include "stdafx.h"
#include <tdk/log/record.hpp>

namespace tdk {
namespace log {

record::record( tdk::log::level l 
	, const tdk::log::category& cate
	, const TCHAR* file
	, const int line
	, const TCHAR* function )
	: level( l )
	, category( cate )
	, file_name( file )
	, line_number( line )
	, function_name( function )
	, time(tdk::date_time::local())
#if defined(_WIN32) 
    , process_id( GetCurrentProcessId() ) 
	, thread_id(GetCurrentThreadId())
#else
    , process_id( getpid())
    , thread_id( pthread_self())
#endif	
{
}

const TCHAR* record::level_string( void ) const {
	switch( level ) {
	case tdk::log::level::log_debug: return _T("D");
	case tdk::log::level::log_warn:  return _T("W");
	case tdk::log::level::log_info:  return _T("I");
	case tdk::log::level::log_error: return _T("E");
	case tdk::log::level::log_fatal: return _T("F");
	}
	return _T("?");
}


}}
