#include "stdafx.h"
#include <tdk/error/error_platform.hpp>
#include <tdk/error/error_category_platform.hpp>
#include <thread>

namespace tdk {
namespace platform{

const std::error_category& category( void ) {
	static tdk::platform::error_category_platform impl;
	return impl;
}

std::error_code error( int ec ) {
	return std::error_code( ec , platform::category());
}

#if defined( _WIN32 )

std::error_code error( void ){
	return std::error_code( GetLastError() , platform::category());
}
std::error_code timeout( WAIT_TIMEOUT , platform::category() );

#elif defined( linux ) || defined ( __linux )

std::error_code error( void ){
	return std::error_code( errno , platform::category());
}
std::error_code timeout( ETIMEDOUT , platform::category() );

#elif defined ( __MACOSX__ ) || defined ( __APPLE__ ) 

#else

#endif

}}