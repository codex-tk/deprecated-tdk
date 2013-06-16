#ifndef __tdk_error_error_tdk_h__
#define __tdk_error_error_tdk_h__

#include <tdk/error/error_code.hpp>
#include <tdk/error/error_category_tdk.hpp>

namespace tdk {
	
const tdk::error_category& category( void );
tdk::error_code error( tdk::errc ec );

extern tdk::error_code tdk_success;
extern tdk::error_code tdk_tls_not_initialized;
extern tdk::error_code tdk_user_abort;
extern tdk::error_code tdk_network_user_abort;
extern tdk::error_code tdk_network_remote_closed;

}

#endif