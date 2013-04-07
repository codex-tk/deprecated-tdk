#ifndef __tdk_thread_data_h__
#define __tdk_thread_data_h__

#include <system_error>

namespace tdk { namespace threading {

struct data {
	std::error_code error;

	static data* get_thread_data( void );
	static void  set_thread_data( data* d );
};

}}

#endif