#ifndef __tdk_platform_error_castgory_h__
#define __tdk_platform_error_castgory_h__

#include <tdk/error/error_category_tdk.hpp>

namespace tdk {
namespace platform {

const std::error_category& category( void );

std::error_code error( int ec );
std::error_code error( void );

extern std::error_code timeout;

}}

#endif