#include "stdafx.h"
#include <tdk/error/error_tdk.hpp>
#include <tdk/error/error_category_decorator.hpp>
#include <tdk/error/error_platform.hpp>

namespace tdk {
	
const std::error_category& category( void ) {
	static tdk::error_category_tdk impl;
	return impl;
}

std::error_code error( tdk::errc ec ){
	return std::error_code( (int)ec , tdk::category());
}

#if defined ( _WIN32 ) 

#elif defined( linux ) || defined ( __linux )
const std::error_category& epoll_category( void ) {
    static tdk::error_category_decorator impl("epoll" , tdk::platform::category());
    return impl;
}
std::error_code epoll_error( int err ){
    return std::error_code( err , epoll_category());
}
#elif defined ( __MACOSX__ ) || defined ( __APPLE__ ) 

#else

#endif

std::error_code tdk_success( (int)errc::tdk_success , tdk::category() );
std::error_code tdk_fail( (int)errc::tdk_fail , tdk::category() );
std::error_code tdk_tls_not_initialized( (int)errc::tdk_tls_not_initialized , tdk::category() );
std::error_code tdk_user_abort( (int)errc::tdk_user_abort , tdk::category() );
std::error_code tdk_network_user_abort( (int)errc::tdk_network_user_abort , tdk::category() );
std::error_code tdk_network_remote_closed( (int)errc::tdk_network_remote_closed , tdk::category() );
std::error_code tdk_epoll_error((int)errc::tdk_epoll_error , tdk::category());
std::error_code tdk_epoll_hang_up((int)errc::tdk_epoll_hang_up , tdk::category());
std::error_code tdk_invalid_call((int)errc::tdk_invalid_call , tdk::category());
std::error_code tdk_invalid_event((int)errc::tdk_invalid_event, tdk::category());


 //std::error_code tdk_timeout( (int)errc::tdk_timeout , tdk::category() );

}
