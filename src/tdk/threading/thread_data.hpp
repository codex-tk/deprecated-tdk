#ifndef __tdk_thread_data_h__
#define __tdk_thread_data_h__

#include <system_error>
#include <tdk/error/error_code.hpp>
#include <tdk/memory/thread_cache.hpp>

namespace tdk { namespace threading {

struct data {
	std::error_code error;
	tdk::tstring per_thread_message;
	tdk::memory::thread_cache cache;
	static data* get_thread_data( void );
	static void  set_thread_data( data* d );
};

}}

#endif