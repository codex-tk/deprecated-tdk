#include <gtest/gtest.h>
#include <sys/epoll.h>
#include <tdk/io/eventfd.hpp>
#include <tdk/error/error_platform.hpp>

TEST( epoll , add_with_out_event ){
    int efd = epoll_create(1);

    tdk::io::eventfd fd;

    epoll_event ev;
    ev.events = EPOLLET | EPOLLONESHOT;
    
    int ret = epoll_ctl( efd , EPOLL_CTL_ADD , fd.handle() , &ev );           
    EXPECT_EQ( ret  , 0 );
    if ( ret < 0 ) {
        printf( "%d %d %s\r\n" , ret 
                , errno 
                ,  tdk::platform::error().message().c_str());
    }
    ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    ret = epoll_ctl( efd , EPOLL_CTL_MOD , fd.handle() , &ev ); 
    EXPECT_EQ( ret , 0 );
    if ( ret < 0 ) {
        printf( "%d %s\r\n"  
                , errno 
                ,  tdk::platform::error().message().c_str());
    }


}
