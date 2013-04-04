#include "stdafx.h"
#include <tdk/error/platform_error.hpp>
#include <tdk/error/windows_error_category.hpp>

namespace tdk {

const std::error_category& platform_category( void ) {
#if defined( _WIN32 ) 
	static tdk::windows_error_category impl;
	return impl;
#else
	// todo
#endif
}

std::error_code platform_error( int ec ) {
	return std::error_code( ec , platform_category());
}

}