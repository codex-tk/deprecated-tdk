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

#if defined(_WIN32) || defined(_WIN64)
struct record {
	tdk::log::level				level;
	const tdk::log::category&	category;
	//std::wstring				message;	// if windows mbs else utf-8 string
	wchar_t						message[k_log_buffer_size];
	const wchar_t*				file_name;
	int							line_number;
	const wchar_t*				function_name;
	tdk::date_time				time;
	int							process_id;
	int							thread_id;

	record( tdk::log::level l 
	, const tdk::log::category& cate
	, const wchar_t* file
	, const int line
	, const wchar_t* function );

	const wchar_t* level_string( void ) const;
};
#else
struct record {
	tdk::log::level				level;
	const tdk::log::category&	category;
	//std::wstring				message;	// if windows mbs else utf-8 string
	char						message[k_log_buffer_size];
	const char*					file_name;
	int							line_number;
	const char*					function_name;
	tdk::date_time				time;
	int							process_id;
	int							thread_id;

	record( tdk::log::level l 
	, const tdk::log::category& cate
	, const char* file
	, const int line
	, const char* function );

	const char* level_string( void ) const;
};
#endif


}}

#endif