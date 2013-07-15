#include <gtest/gtest.h>
#include <tdk/io/engine_epoll.hpp>
#include <string>
#include <thread>
#include <chrono>

TEST( tdk_io_engine , t1 ) {
    tdk::io::engine engine;

    engine.post( []{
                printf( "1\r\n");
            });

    engine.post( []{
                printf( "2\r\n");
            });
    printf( "first wait %d\r\n", engine.wait( tdk::time_span::infinite()));

    printf( "second wait %d\r\n", engine.wait( tdk::time_span::from_seconds(1)));

    engine.post( [] {
            printf( "3\r\n");
            });

    engine.wait( tdk::time_span::infinite());


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

    tdk::io::ip::address google_addr;//( "" , 81 );
    printf( "%s %d\r\n" , google_addr.ip_address().c_str() , google_addr.port());

    fd.async_connect( google_addr ,  []( const std::error_code& ec ) {
                    if ( ec ) {
                        printf( "Connect %d %s\r\n" ,ec.value(), ec.message().c_str());
                    } else {
                        printf( "Connect Success\r\n");
                    }
                  });

    
    e.wait( tdk::time_span::infinite());



}




