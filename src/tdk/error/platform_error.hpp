#ifndef __tdk_platform_error_castgory_h__
#define __tdk_platform_error_castgory_h__

#include <tdk/error/error_code.hpp>

namespace tdk {
	
const tdk::error_category& platform_category( void );
const tdk::error_category& tdk_category( void );

tdk::error_code platform_error( int ec );
tdk::error_code platform_error( void );

tdk::error_code tdk_error( int ec );

}

#endif