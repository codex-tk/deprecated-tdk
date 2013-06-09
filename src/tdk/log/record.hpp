#ifndef __tdk_log_record_h__
#define __tdk_log_record_h__

#include <tdk/log/category.hpp>
#include <tdk/time/date_time.hpp>

namespace tdk {
namespace log {
namespace {

const int k_log_buffer_size = 4192;

}

enum class level {
	log_debug = 0,
	log_info  ,
	log_warn  ,
	log_error ,
	log_fatal ,
};

struct record {
	tdk::log::level				level;
	const tdk::log::category&	category;
	TCHAR						message[k_log_buffer_size];
	const TCHAR*				file_name;
	int							line_number;
	const TCHAR*				function_name;
	tdk::date_time				time;
	int							process_id;
	int							thread_id;

	record( tdk::log::level l 
	, const tdk::log::category& cate
	, const TCHAR* file
	, const int line
	, const TCHAR* function );

	const TCHAR* level_string( void ) const;
};


}}

#endif