#include <gtest/gtest.h>
#include <tdk/io/ip/socket.hpp>
#include <string>
#include <tdk/buffer/memory_block.hpp>
#include <tdk/log/logger.hpp>

TEST( tdk_io_ip_socket , dt ) {
    std::vector< tdk::io::ip::address > addr;
    EXPECT_TRUE( tdk::io::ip::address::resolve( "google.com" , 80 , addr ));
    for ( auto it : addr ) {
        tdk::io::ip::socket fd;
        ASSERT_TRUE(fd.open_tcp( it.family()));

        bool ret = fd.connect_time_out( it 
                , tdk::time_span::from_seconds(1));

        ASSERT_TRUE(ret) << errno;

        char req[] = "GET /index HTTP/1.0\r\n\r\n";

        int r = fd.send( req , strlen(req) );
        ASSERT_EQ( r , strlen( req ));
        
        tdk::buffer::memory_block mb( 4096 );

        r = fd.recv_time_out( mb.wr_ptr() , mb.space() 
                , tdk::time_span::from_seconds(1));
        ASSERT_TRUE( r > 0 ) << errno;
        mb.wr_ptr( r );
        mb << 0;
        fd.close();
        LOG_D( tdk::log::logger::get_instance( "main")
                , "%s" 
                , mb.rd_ptr());
        break;
    }


}

