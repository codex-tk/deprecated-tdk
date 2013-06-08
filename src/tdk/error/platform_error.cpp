#include "stdafx.h"
#include <tdk/error/platform_error.hpp>
#include <tdk/error/error_category_win32.hpp>
#include <tdk/error/error_category_tdk.hpp>
#include <thread>

namespace tdk {

const tdk::error_category& platform_category( void ) {
	static tdk::error_category_platform impl;
	return impl;
}

const tdk::error_category& tdk_category( void ) {
	static tdk::error_category_tdk impl;
	return impl;
}

tdk::error_code platform_error( int ec ) {
	return tdk::error_code( ec , platform_category());
}

tdk::error_code platform_error( void ){
	return tdk::error_code( GetLastError() , platform_category());
}


tdk::error_code tdk_error( tdk::errc ec ){
	return tdk::error_code( (int)ec , tdk_category());
}

}