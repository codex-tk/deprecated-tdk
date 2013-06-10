#ifndef __tdk_platform_error_castgory_h__
#define __tdk_platform_error_castgory_h__

#include <tdk/error/error_code.hpp>
#include <tdk/error/error_category_tdk.hpp>

namespace tdk {
namespace platform {

const tdk::error_category& category( void );

tdk::error_code error( int ec );
tdk::error_code error( void );

extern tdk::error_code timeout;

}}

#endif