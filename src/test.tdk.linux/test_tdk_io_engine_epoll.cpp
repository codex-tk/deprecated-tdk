#include <gtest/gtest.h>
#include <tdk/io/engine_epoll.hpp>
#include <string>
#include <thread>
#include <chrono>
#include <tdk/buffer/memory_block.hpp>

TEST( tdk_io_engine , t1 ) {
    tdk::io::engine engine;
    int testval1 = 0;
    int testval2 = 0;
    engine.post( [&]{
                testval1 = 1;
            });

    engine.post( [&]{
                testval2 = 2;
            });

    engine.wait( tdk::time_span::infinite());

    engine.wait( tdk::time_span::from_seconds(1));

    ASSERT_EQ( testval1 , 1 );
    ASSERT_EQ( testval2 , 2 );

    int testval3 = 0;

    engine.post( [&] {
                testval3 = 1;
            });


    engine.wait( tdk::time_span::infinite());


    ASSERT_EQ( testval3 , 1);
/*
    auto thread_handler = [&]{
            std::chrono::milliseconds ms(1000);
            std::this_thread::sleep_for( ms );
            printf( "Notify\r\n");
            engine.test_notify();

    };

    std::thread t( thread_handler );

    int nev = engine.wait( tdk::time_span::infinite());
    
    printf( "Wake %d\r\n" , nev );
    t.join();

    t = std::thread(thread_handler);

    nev = engine.wait( tdk::time_span::infinite());
    printf( "Wake %d\r\n" , nev );
   
    t.join();    
*/
}

#include <tdk/io/ip/tcp/socket_epoll.hpp>

TEST( tdk_io_engine , async_connect ) {
    tdk::io::engine e;
    tdk::io::ip::tcp::socket fd( e );

    tdk::io::ip::address google_addr( "google.co.kr" , 80 );
    printf( "%s %d\r\n" , google_addr.ip_address().c_str() , google_addr.port());

    fd.async_connect( google_addr ,  []( const std::error_code& ec ) {
                    if ( ec ) {
                        //printf( "Connect %d %s\r\n" ,ec.value(), ec.message().c_str());
                        printf( "Connect Fail");
                    } else {
                        //printf( "Connect Success\r\n");
                    }
                  });

    e.wait( tdk::time_span::infinite());

    char req[] = "GET /index HTTP/1.0\r\n\r\n";
    
    tdk::io::buffer_adapter buf( req , strlen(req));
    fd.async_send( buf , []( const std::error_code& err , int iobyte) {
                if ( err ) {
                    printf( "SendError %s\r\n" , err.message().c_str());
                } else {
                    printf( "Send!!\r\n");
                }

            });
    e.wait( tdk::time_span::infinite());

    tdk::buffer::memory_block mb( 4096 );

    
    tdk::io::buffer_adapter recvbuf( mb.wr_ptr() , mb.space());
    fd.async_recv( recvbuf , [] ( const std::error_code& err , int iobyte ){
                 if ( err ) {
                    printf( "RecvError %s\r\n" , err.message().c_str());
                } else {
                    printf( "REcv %d!!\r\n", iobyte );
                }
            });

    printf( "1");
    e.wait( tdk::time_span::infinite());
}




