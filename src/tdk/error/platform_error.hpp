#ifndef __tdk_platform_error_castgory_h__
#define __tdk_platform_error_castgory_h__

#include <tdk/error/error_code.hpp>
#include <tdk/error/tdk_error_category.hpp>
namespace tdk {
	
const tdk::error_category& platform_category( void );
const tdk::error_category& tdk_category( void );

tdk::error_code platform_error( int ec );
tdk::error_code platform_error( void );

tdk::error_code tdk_error( tdk::errc ec );

}

#endif