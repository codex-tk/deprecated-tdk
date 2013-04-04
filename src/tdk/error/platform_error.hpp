#ifndef __tdk_platform_error_castgory_h__
#define __tdk_platform_error_castgory_h__

#include <system_error>

namespace tdk {

const std::error_category& platform_category( void );

std::error_code platform_error( int ec );

}

#endif