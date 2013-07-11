#include <cstdio>
#include <gtest/gtest.h>
#include <tdk/tdk.hpp>
#include <tdk/log/logger.hpp>
#include <tdk/log/writer/stderr_writer.hpp>

int main( int argc , char* argv[] ) {

    testing::InitGoogleTest( &argc , argv );

    tdk::init();

    tdk::log::logger logger = tdk::log::logger::get_instance( "main");

    tdk::log::writer_ptr ptr = tdk::log::stderr_writer::instance();
    logger.add_writer( ptr );

    LOG_D( logger , "test %s" , "test1" );
    printf( "test\r\n");
    return RUN_ALL_TESTS();
}
