#include "stdafx.h"

#include <tdk/error/last_error.hpp>
#include <tdk/error/error_category_tdk.hpp>
#include <tdk/error/error_tdk.hpp>

#include <tdk/threading/thread_data.hpp>

#include <thread>

namespace tdk {

void set_last_error( const std::error_code& ec ) {
	threading::data* td = threading::this_thread::get_data();
	if ( td ) {
		td->error = ec;
	}
}

const std::error_code& get_last_error( void ) {
	threading::data* td = threading::this_thread::get_data();
	if ( td ) {
		return td->error;
	}
	static std::error_code tls_not_initialized
		= tdk::error( tdk::errc::tdk_tls_not_initialized ); 
	return tls_not_initialized;
}

}