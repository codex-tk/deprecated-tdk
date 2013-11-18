#include <gtest/gtest.h>
#include <tdk/tdk.hpp>
#include <tdk/error/error_platform.hpp>
#include <system_error>
#include <tdk/log/logger.hpp>

TEST( tdk_error , platform ){ 
    std::error_code ec = tdk::platform::error();
    LOG_D( tdk::log::logger::get_instance("main")
            , "Platform Error %s"
            , ec.message().c_str());
}
