#ifndef __tdk_thread_data_h__
#define __tdk_thread_data_h__

#include <system_error>
#include <tdk/tdk.hpp>

namespace tdk { namespace threading {

struct data {
	std::error_code error;
	tdk::tstring per_thread_message;
	//tdk::memory::thread_cache cache;
};

class this_thread {
public:
	static data* get_data( void );
	static void  set_data( data* d );
};

}}

#endif
