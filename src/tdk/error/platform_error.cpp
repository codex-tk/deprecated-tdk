#include "stdafx.h"
#include <tdk/error/platform_error.hpp>
#include <tdk/error/windows_error_category.hpp>
#include <thread>

namespace tdk {

const tdk::error_category& platform_category( void ) {
#if defined( _WIN32 ) 
	static tdk::windows_error_category impl;
	return impl;
#else
	// todo
#endif
}

tdk::error_code platform_error( int ec ) {
	return tdk::error_code( ec , platform_category());
}

}