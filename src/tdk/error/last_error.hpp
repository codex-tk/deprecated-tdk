#ifndef __tdk_last_error_h__
#define __tdk_last_error_h__

//#include <system_error>
#include <tdk/error/error_code.hpp>
#include <tdk/error/platform_error.hpp>

namespace tdk {

void set_last_error( const tdk::error_code& ec );
const tdk::error_code& get_last_error( void );

}

#endif