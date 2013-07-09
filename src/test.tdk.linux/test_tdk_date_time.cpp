#include <gtest/gtest.h>
#include <tdk/time/date_time.hpp>
#include <string>

TEST( tdk_date_time , dt ) {
/*
    struct timespec ts;
    if ( clock_gettime( CLOCK_REALTIME , &ts )==-1){
        printf( "Error\r\n");
    } else {
        printf( "%d %d\r\n" , ts.tv_sec , ts.tv_nsec / 1000  );
        printf( "%d\r\n" , time(nullptr));
    }
    tdk::date_time dt1( tdk::time::tick::from( time(nullptr)));
*/

    tdk::date_time dt = tdk::date_time::local();
    dt = tdk::date_time::utc();

    tdk::time_span diff = tdk::date_time::local() - tdk::date_time::utc();
    // gmt + 9
    ASSERT_EQ( diff.total_hours() , 9 );
}


TEST( tdk_date_time , utc_local ) {
    tdk::date_time utc = tdk::date_time::utc();
    tdk::date_time local = tdk::date_time::local( utc );

    ASSERT_EQ( (local - utc).total_hours() , 9 );
}
