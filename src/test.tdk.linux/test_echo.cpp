#include <gtest/gtest.h>
#include <tdk/io/engine_epoll.hpp>
#include <tdk/io/ip/tcp/socket_epoll.hpp>
#include <tdk/log/logger.hpp>
#include <thread>
#include <list>

std::list< tdk::io::ip::tcp::socket* > _sockets;
tdk::threading::spin_lock _lock;
std::atomic<int> current;
void on_recv( tdk::io::ip::tcp::socket* fd , 
        const tdk::buffer::memory_block& readmb ,
        const std::error_code& ec ,
        int iobyte )
{
    if ( ec ){
        printf( "Closed %s\r\n" , ec.message().c_str());
        fd->close( [fd]{
                    do {
                    
                        tdk::threading::scoped_lock<> gaurd( _lock );
                        _sockets.remove( fd );
                    } while(0);
                    delete fd;
                    current.fetch_sub(1);
                    printf( "Current %d\r\n" , current.load());
                });
        return;
    }
    tdk::buffer::memory_block writemb( readmb );
    writemb.wr_ptr( iobyte );
    tdk::io::buffer_adapter buf( writemb.rd_ptr() , writemb.length());
    fd->async_send( buf , [writemb]( const std::error_code& ec , int io){
            });
    tdk::buffer::memory_block mb(4096);
    tdk::io::buffer_adapter buf2( mb.wr_ptr() , mb.space());
    fd->async_recv( buf2 , [fd,mb]( const std::error_code& ec , int io ){
                on_recv(fd  , mb , ec , io );
            });
}


void on_accept( 
        tdk::io::ip::tcp::socket* acceptor
        , tdk::io::ip::tcp::socket* s
        , const std::error_code& e )
{
    if ( e ) {
        LOG_D( tdk::log::logger::get_instance("main")
                , "Accept Error %s" , e.message().c_str());
        delete s;
    } else {
        current.fetch_add(1);
        LOG_D( tdk::log::logger::get_instance("main")
                , "Accept Success", 0 );
        tdk::buffer::memory_block mb(4096);
        tdk::io::buffer_adapter buf( mb.wr_ptr() , mb.space());
        s->async_recv( buf , [s,mb]( const std::error_code& ec , int io ){
                    on_recv( s , mb , ec , io );
                });
        tdk::threading::scoped_lock<> guard( _lock );
        _sockets.push_back(s);
    }
    
    
    tdk::io::ip::tcp::socket* fd = new tdk::io::ip::tcp::socket( acceptor->engine());
    acceptor->async_accept( *fd , [ acceptor , fd ]( const std::error_code& ec ) {
                on_accept( acceptor , fd , ec );
            });
    
}

void on_close_all( tdk::io::engine* e  ){
    do {
        tdk::threading::scoped_lock<> gaurd( _lock );
        for ( auto it : _sockets ){
            (*it).close([]{});
        }
    }while(0);
    e->schedule( tdk::date_time::utc() + tdk::time_span::from_minutes(1)
            ,[e]( const std::error_code& ec ){
                on_close_all(e);
            });
}

void on_print( tdk::io::engine* e ){
    printf("Current Connection %d\r\n", current.load());
    e->schedule( tdk::date_time::utc() + tdk::time_span::from_seconds(10)
            ,[e]( const std::error_code& ec ){
                on_print(e);
            });
}

TEST( echo , e ) {
    tdk::io::engine e;

    tdk::io::ip::tcp::socket acceptor(e);
    ASSERT_TRUE( acceptor.open_accept( tdk::io::ip::address::any( 10012 )));
/*
    ASSERT_TRUE( acceptor.open_tcp( AF_INET ));
    tdk::io::ip::tcp::socket::option::reuse_address reuse;
    tdk::io::ip::tcp::socket::option::non_blocking nb;
    acceptor.set_option( reuse );
    acceptor.set_option( nb );
    ASSERT_TRUE( acceptor.bind( tdk::io::ip::address::any(10012)));
    ASSERT_TRUE( acceptor.listen());
  */          
    tdk::io::ip::tcp::socket*  peer = new tdk::io::ip::tcp::socket(e);
    acceptor.async_accept( *peer , [&]( const std::error_code& ec ){
                on_accept( &acceptor , peer , ec );
            });

    current.store(0);
    std::thread t( [&]{
            printf( "%d\r\n" , pthread_self());
                e.run();
            });

    /*
    std::thread t2([]{
                while( true ) {
                    std::this_thread::sleep_for( std::chrono::seconds(5));
                    printf("Current Connection %d\r\n", current.load());

                }
                });
    std::thread t3([] {
                std::this_thread::sleep_for( std::chrono::minutes(1));
                printf( "Close All" );
                tdk::threading::scoped_lock<> gaurd( _lock );
                for ( auto it : _sockets ){
                    (*it).close([]{});
                }

            });
            */

   e.schedule( tdk::date_time::utc() + tdk::time_span::from_minutes(1)
            , [&e]( const std::error_code& ec ) {
                on_close_all(&e);
            });
   e.schedule( tdk::date_time::utc() + tdk::time_span::from_seconds(10)
            , [&e]( const std::error_code& ec ) {
                on_print(&e);
            });
   printf( "%d\r\n" , pthread_self());
    e.run();
}
