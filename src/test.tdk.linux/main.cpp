#include <cstdio>
#include <gtest/gtest.h>
#include <tdk/tdk.hpp>

int main( int argc , char* argv[] ) {

    testing::InitGoogleTest( &argc , argv );

    tdk::init();

    printf( "test\r\n");
    return RUN_ALL_TESTS();
}
