#ifndef __tdk_thread_data_h__
#define __tdk_thread_data_h__

#include <system_error>
#include <tdk/error/error_code.hpp>

namespace tdk { namespace threading {

struct data {
	tdk::error_code error;
	tdk::tstring per_thread_message;
	static data* get_thread_data( void );
	static void  set_thread_data( data* d );
};

}}

#endif