#ifndef __tdk_log_record_h__
#define __tdk_log_record_h__

#include <tdk/log/category.hpp>
#include <tdk/time/date_time.hpp>

namespace tdk {
namespace log {

enum class LEVEL {
	LOG_DEBUG = 0,
	LOG_INFO  ,
	LOG_WARN  ,
	LOG_ERROR ,
	LOG_FATAL ,
};
struct record {
	tdk::log::LEVEL				level;
	const tdk::log::category&	category;
	std::string					message;	// utf-8 string
	const char*					file_name;
	int							line_number;
	const char*					function_name;
	tdk::date_time				time;
	int							process_id;
	int							thread_id;

	record( tdk::log::LEVEL l 
	, const tdk::log::category& cate
	, const std::string& msg 
	, const char* file
	, const int line
	, const char* function );

	const char* level_string( void ) const;
};

}}

#endif