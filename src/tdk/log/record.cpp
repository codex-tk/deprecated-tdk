#include "stdafx.h"
#include <tdk/log/record.hpp>

namespace tdk {
namespace log {

#if defined(_WIN32) || defined(_WIN64)
record::record( tdk::log::level l 
	, const tdk::log::category& cate
	, const std::wstring& msg 
	, const wchar_t* file
	, const int line
	, const wchar_t* function )
	: level( l )
	, category( cate )
	, message( msg )
	, file_name( file )
	, line_number( line )
	, function_name( function )
	, time(tdk::date_time::local())
#if defined(_WIN32) || defined(_WIN64)
	, process_id( GetCurrentProcessId() ) 
	, thread_id(GetCurrentThreadId())
#else

#endif	
{
}

const wchar_t* record::level_string( void ) const {
	switch( level ) {
	case tdk::log::level::log_debug: return L"D";
	case tdk::log::level::log_warn:  return L"W";
	case tdk::log::level::log_info:  return L"I";
	case tdk::log::level::log_error: return L"E";
	case tdk::log::level::log_fatal: return L"F";
	}
	return L"?";
}

#else

record::record( tdk::log::level l 
	, const tdk::log::category& cate
	, const std::string& msg 
	, const char* file
	, const int line
	, const char* function )
	: level( l )
	, category( cate )
	, message( msg )
	, file_name( file )
	, line_number( line )
	, function_name( function )
	, time(tdk::date_time::local())
	, process_id( 0 ) 
	, thread_id( 0 )
{
}

const char* record::level_string( void ) const {
	switch( level ) {
	case tdk::log::level::log_debug: return "D";
	case tdk::log::level::log_warn:  return "W";
	case tdk::log::level::log_info:  return "I";
	case tdk::log::level::log_error: return "E";
	case tdk::log::level::log_fatal: return "F";
	}
	return "?";
}

#endif
}}