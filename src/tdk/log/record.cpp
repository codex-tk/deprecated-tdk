#include "stdafx.h"
#include <tdk/log/record.hpp>

namespace tdk {
namespace log {

record::record( tdk::log::LEVEL l 
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
#if defined(_WIN32) || defined(_WIN64)
	, process_id( GetCurrentProcessId() ) 
	, thread_id(GetCurrentThreadId())
#else

#endif	
{
}

const char* record::level_string( void ) const {
	switch( level ) {
	case tdk::log::LEVEL::LOG_DEBUG: return "D";
	case tdk::log::LEVEL::LOG_WARN:  return "W";
	case tdk::log::LEVEL::LOG_INFO:  return "I";
	case tdk::log::LEVEL::LOG_ERROR: return "E";
	case tdk::log::LEVEL::LOG_FATAL: return "F";
	}
	return "?";
}

}}