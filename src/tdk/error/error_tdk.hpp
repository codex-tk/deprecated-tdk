#ifndef __tdk_error_error_tdk_h__
#define __tdk_error_error_tdk_h__

#include <tdk/error/error_category_tdk.hpp>

namespace tdk {
	
const std::error_category& category( void );
std::error_code error( tdk::errc ec );

#if defined ( _WIN32 ) 

#elif defined( linux ) || defined ( __linux )
const std::error_category& epoll_category( void );
std::error_code epoll_error( int err );
#elif defined ( __MACOSX__ ) || defined ( __APPLE__ ) 

#else

#endif


extern std::error_code tdk_success;
extern std::error_code tdk_tls_not_initialized;
extern std::error_code tdk_user_abort;
extern std::error_code tdk_network_user_abort;
extern std::error_code tdk_network_remote_closed;

extern std::error_code tdk_epoll_error;
extern std::error_code tdk_epoll_hang_up;

extern std::error_code tdk_invalid_call;
// 
//extern std::error_code tdk_timeout;

}

#endif