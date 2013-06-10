#include "stdafx.h"
#include <tdk/error/error_platform.hpp>
#include <tdk/error/error_category_platform.hpp>
#include <thread>

namespace tdk {
namespace platform{

const tdk::error_category& category( void ) {
	static tdk::platform::error_category_platform impl;
	return impl;
}

tdk::error_code error( int ec ) {
	return tdk::error_code( ec , platform::category());
}

}}