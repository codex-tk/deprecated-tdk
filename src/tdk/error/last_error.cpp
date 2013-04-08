#include "stdafx.h"
#include <tdk/error/last_error.hpp>
#include <tdk/threading/thread_data.hpp>
#include <thread>

namespace tdk {

void set_last_error( const tdk::error_code& ec ) {
	threading::data::get_thread_data()->error = ec;
}

const tdk::error_code& get_last_error( void ) {
	return threading::data::get_thread_data()->error;
}

}