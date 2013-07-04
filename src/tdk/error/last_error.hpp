#ifndef __tdk_last_error_h__
#define __tdk_last_error_h__

//#include <system_error>

#include <tdk/error/error_platform.hpp>

namespace tdk {

void set_last_error( const std::error_code& ec );
const std::error_code& get_last_error( void );

}

#endif