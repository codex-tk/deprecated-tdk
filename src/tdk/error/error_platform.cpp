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

}}