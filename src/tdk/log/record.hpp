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
#if defined ( _WIN32 ) 
    typedef int pid_type;
    typedef int tid_type;
#elif defined( linux ) || defined ( __linux )
    typedef pid_t     pid_type;
    typedef pthread_t tid_type;
#elif defined ( __MACOSX__ ) || defined ( __APPLE__ ) 

#else

#endif
	tdk::log::level				level;
	const tdk::log::category&	category;
	TCHAR						message[k_log_buffer_size];
	const TCHAR*				file_name;
	int							line_number;
	const TCHAR*				function_name;
	tdk::date_time				time;
    pid_type					process_id;
	tid_type					thread_id;
	record( tdk::log::level l 
	, const tdk::log::category& cate
	, const TCHAR* file
	, const int line
	, const TCHAR* function );

	const TCHAR* level_string( void ) const;
};


}}

#endif
