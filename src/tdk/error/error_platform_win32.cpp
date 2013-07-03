#include "stdafx.h"
#include <tdk/error/error_platform.hpp>
#include <tdk/error/error_category_platform.hpp>

namespace tdk {
namespace platform{

std::error_code error( void ){
	return std::error_code( GetLastError() , platform::category());
}

static std::error_code timeout( WAIT_TIMEOUT , platform::category() );

}}