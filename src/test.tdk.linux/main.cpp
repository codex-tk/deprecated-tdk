#include <cstdio>
#include <gtest/gtest.h>
#include <tdk/tdk.hpp>
#include <tdk/log/logger.hpp>
#include <tdk/log/writer/stderr_writer.hpp>
#include <sys/epoll.h>
#include <tdk/error/error_platform.hpp>
#include <tdk/io/eventfd.hpp>
#include <tdk/io/ip/socket.hpp>
#include <tdk/io/timerfd.hpp>
#include <thread>
int main( int argc , char* argv[] ) {

    testing::InitGoogleTest( &argc , argv );

    tdk::init();

    tdk::log::logger logger = tdk::log::logger::get_instance( "main");

    tdk::log::writer_ptr ptr = tdk::log::stderr_writer::instance();
    logger.add_writer( ptr );

/*    int fd = epoll_create(1);

    tdk::io::timerfd tfd;
   
    epoll_event evt;
    evt.events = EPOLLIN;

    int ret = epoll_ctl( fd , EPOLL_CTL_ADD , tfd.handle() , &evt );
    printf( "%d %d %d %s\r\n" , ret , errno , tdk::platform::error().value() ,  tdk::platform::error().message().c_str());



    tfd.set( tdk::date_time::utc() + tdk::time_span::from_seconds(100));
    std::thread t( [&] {
        LOG_D( logger , "Begin %s" , tdk::date_time::local().to_string().c_str());
        ret =  epoll_wait( fd ,  &evt , 1 , -1 );
        uint64_t val = 0;

        int ret = read( tfd.handle() , &val , sizeof(val));
        LOG_D( logger , "end %d %d %s" , ret , val ,  tdk::date_time::local().to_string().c_str());
        });

    getchar();

    tfd.set( tdk::date_time::utc() - tdk::time_span::from_seconds(101));

    printf( "Set!!\r\n");

    getchar();
    return 0;
  */  
    LOG_D( logger , "test %s" , "test1" );
    return RUN_ALL_TESTS();
/*
    int fd = epoll_create(1);

    tdk::io::eventfd efd;
   
    epoll_event evt;
    evt.events = EPOLLIN;

    epoll_event evt2;
    evt.events = EPOLLOUT;

    int ret = epoll_ctl( fd , EPOLL_CTL_ADD , efd.handle() , &evt );
    printf( "%d %d %d %s\r\n" , ret , errno , tdk::platform::error().value() ,  tdk::platform::error().message().c_str());
    int ret2 = epoll_ctl( fd , EPOLL_CTL_ADD , efd.handle() , &evt2 );
    printf( "%d %d %d %s\r\n" , ret2 , errno , tdk::platform::error().value() ,  tdk::platform::error().message().c_str());
*/
/*
    int epoll_fd = epoll_create(1);
    
    if ( epoll_fd <= 0 ) 
        printf( "Epoll Create Fail\r\n" );

    tdk::io::ip::socket fd;
    if( !fd.open_tcp( AF_INET)){
        printf( "Open Tcp Fail\r\n");
    }

    epoll_event ev;
    ev.events = EPOLLOUT | EPOLLET | EPOLLONESHOT;
    ev.data.ptr = nullptr;

    if ( epoll_ctl( epoll_fd , EPOLL_CTL_ADD , fd.handle() , &ev ) == -1 ) {
        printf( "Epoll Ctl Error\r\n");
    }

    printf( "GetAddr\r\n");
    tdk::io::ip::address addr( "google.co.kr" , 80 );
    tdk::io::ip::socket::option::non_blocking nb;
    fd.set_option(nb) ;
    bool cret = fd.connect( addr );

    if ( cret ) {
        printf( "Connnect Success\r\n");
    }else{
        if ( tdk::platform::error().value() == EINPROGRESS ) {
            printf( "Connect InProgress\r\n" );
        } else {
            printf( "Connect Fail %d\r\n" , errno );
        }
    }

    printf( "Epoll_Wait\r\n");
    int ret = epoll_wait( epoll_fd , &ev , 1 , -1 );
    printf ( "Epoll Wait ret %d\r\n" , ret );

    if ( ret == 1 ) {
        if ( ev.events & EPOLLOUT ) {
            printf( "OUTEVENT!!\r\n" );
            tdk::io::ip::socket::option::error er;
            if ( fd.get_option( er )){
                printf( "GetOption %d \r\n" , er.value() );
            }
        }
    }

    fd.close();
    close( epoll_fd );

*/
//    return RUN_ALL_TESTS();
}
