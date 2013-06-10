#include "stdafx.h"
#include <tdk/error/error_platform.hpp>
#include <tdk/error/error_category_platform.hpp>

namespace tdk {
namespace platform{

tdk::error_code error( void ){
	return tdk::error_code( GetLastError() , platform::category());
}

static tdk::error_code timeout( WAIT_TIMEOUT , platform::category() );

}}