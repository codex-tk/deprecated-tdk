#include <gtest/gtest.h>
#include <tdk/io/ip/address.hpp>
#include <string>

TEST( tdk_io_ip_address , t1 ) {
       std::vector< tdk::io::ip::address > addr;
       EXPECT_TRUE( tdk::io::ip::address::resolve( "google.com" , 80 , addr ));
       ASSERT_FALSE( addr.empty());
       /*
       for ( auto it : addr ) {
           printf( "%s\r\n" , it.ip_address().c_str());
       }*/

}



