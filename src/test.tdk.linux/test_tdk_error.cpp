#include <gtest/gtest.h>
#include <tdk/tdk.hpp>
#include <tdk/error/error_platform.hpp>
#include <system_error>
TEST( tdk_error , platform ){ 
    printf( "error!\r\n");
    std::error_code ec = tdk::platform::error();
    printf( "error : %s\r\n" , ec.message().c_str());
}
