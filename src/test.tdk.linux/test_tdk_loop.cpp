#include <gtest/gtest.h>
#include <tdk/loop/loop.hpp>
#include <tdk/loop/epoll.hpp>
#include <tdk/io/timerfd.hpp>
#include <sys/epoll.h>
#include <thread>
#include <tdk/io/ip/tcp/channel_linux.hpp>

class timer_epoll{
public:
    timer_epoll(  tdk::loop& loop )
    {
        _handler.set( &timer_epoll::on_timer , this );
        _loop = &loop;
    }

    ~timer_epoll( void ) {

    }

    void set( const tdk::date_time& dt ) {
        _fd.set( dt );
        _loop->implementation().reg( _fd.handle() , EPOLLIN | EPOLLONESHOT , &_handler );
        _loop->add_active();
    }

    static void on_timer( tdk::req_handle* req ) {
        static_cast< timer_epoll* >( req->data() )->on_timer_impl( 
                    static_cast< tdk::epoll::event_handler* >(req)->events()
                );
    }

    void on_timer_impl( int evt ) {
        if ( tdk::loop::current_loop() == _loop ) 
            _test_val = evt;
        else 
            _test_val = 0;
        _loop->remove_active();
    }

    int handle( void ) {
        return _fd.handle();
    }
public:
    int _test_val;
    tdk::io::timerfd _fd;
    tdk::epoll::event_handler _handler;
    tdk::loop* _loop;
};

TEST( loop , impl ) {
    tdk::loop loop;

    timer_epoll timer( loop );
    timer.set( tdk::date_time::utc() + tdk::time_span::from_milli_seconds(100));

    ASSERT_EQ( tdk::loop::current_loop() , nullptr );

    loop.run();

    ASSERT_EQ( tdk::loop::current_loop() , nullptr );
    ASSERT_EQ( timer._test_val , EPOLLIN );
}


TEST( loop , epoll_wakeup ) {
    tdk::loop loop;
    auto thread_handler = [&](){
        for ( int i = 0 ; i < 5 ; ++i ) {
            loop.add_active();
            loop.run();
        }

    };
    std::thread t( thread_handler );

    for ( int i = 0 ; i < 5 ; ++i ) {
        usleep(100 * 1000);
        loop.execute( [&]{
                    loop.remove_active();
                });
    }
    t.join();
}
static void on_connect( tdk::req_handle* req ){
    tdk::io::ip::tcp::connect_req* creq = static_cast< tdk::io::ip::tcp::connect_req* >( req );
    if ( creq->error() ) {
        printf( "Connect Error %d %s\r\n" , creq->error().value() , creq->error().message().c_str());
        return;
    }
    printf( "Connect\r\n" );
}

static void on_close( tdk::req_handle* req ){
    printf( "Close\r\n");
}

class timer_epoll2{
public:
    timer_epoll2(  tdk::loop& loop )
    {
        _handler.set( &timer_epoll2::on_timer , this );
        _loop = &loop;
    }

    ~timer_epoll2( void ) {

    }

    void set( const tdk::date_time& dt ) {
        _fd.set( dt );
        _loop->implementation().reg( _fd.handle() , EPOLLIN | EPOLLONESHOT , &_handler );
        _loop->add_active();
    }

    static void on_timer( tdk::req_handle* req  ) {
        static_cast< timer_epoll2* >(req->data())->on_timer_impl( 
                    static_cast< tdk::epoll::event_handler* >( req )->events()
                );
    }

    void on_timer_impl( int evt ) {
        printf( "timer\r\n");
        creq.callback( &on_close );
        creq.data(nullptr);
        chan->async_close( &creq );
        _loop->remove_active();
    }

    int handle( void ) {
        return _fd.handle();
    }
public:
    tdk::io::timerfd _fd;
    tdk::epoll::event_handler _handler;
    tdk::loop* _loop;
    tdk::io::ip::tcp::channel* chan;
    tdk::io::ip::tcp::close_req creq;
};


TEST( loop , connect ) {
    tdk::loop loop;
    tdk::io::ip::tcp::channel channel( loop );
    std::vector< tdk::io::ip::address > addr;

    addr.push_back( tdk::io::ip::address( "192.168.245.123" , 89 ));
    EXPECT_TRUE( tdk::io::ip::address::resolve( "google.com" , 80 , addr ));
    tdk::io::ip::tcp::connect_req req;
    req.callback( &on_connect );
    req.data( nullptr );
    channel.async_connect( &req ,  addr );

    timer_epoll2 timer( loop );
    timer.set( tdk::date_time::utc() + tdk::time_span::from_seconds(2));
    timer.chan = &channel;
    /*
    tdk::io::ip::tcp::close_req creq;
    creq.callback( &on_close );
    creq.data(nullptr);
    channel.async_close( &creq );*/

    printf( "run\r\n");
    loop.run();
    printf( "run endd\r\n");
}

void on_read( tdk::req_handle* req ) {
    tdk::io::ip::tcp::read_req* r = static_cast<
        tdk::io::ip::tcp::read_req* >( req );
    printf( "Read %d\r\n" , r->io_bytes() );
    uint8_t* data = static_cast< uint8_t*>(req->data());
    delete [] data;
    delete r;
}


void on_connect2( tdk::req_handle* req ){
    tdk::io::ip::tcp::connect_req* c = 
        static_cast< tdk::io::ip::tcp::connect_req* >( req );
    
    tdk::io::ip::tcp::read_req* read_req = 
        new tdk::io::ip::tcp::read_req();
    
    read_req->set( &on_read , new uint8_t[4096] );

    tdk::io::buffer_adapter adapter( read_req->data() , 4096 );
    c->channel().async_read( 
            read_req , adapter );

    char http_req[] = "GET /index HTTP/1.0\r\n\r\n";
    c->channel().socket_layer().send( 
        http_req , strlen(http_req));    
}

TEST( loop , get ) {
    tdk::loop loop;
    tdk::io::ip::tcp::channel channel( loop );
    std::vector< tdk::io::ip::address > addr;

    EXPECT_TRUE( tdk::io::ip::address::resolve( "google.com" , 80 , addr ));
    tdk::io::ip::tcp::connect_req req;
    req.callback( &on_connect2 );
    req.data( nullptr );
    channel.async_connect( &req ,  addr );
    loop.run();
}


#include <tdk/io/ip/tcp/acceptor_linux.hpp>


void on_accept( tdk::req_handle* r ) {
    tdk::io::ip::tcp::accept_req* req = 
        static_cast< tdk::io::ip::tcp::accept_req* >( r );
    if ( req->error() ) {
        printf( "OnAccept Error %s\r\n" , req->error().message().c_str());
    } else {
        printf( "OnAccept %s\r\n" , req->address().ip_address().c_str());
    }
    close( req->accepted_fd());
    tdk::io::ip::tcp::acceptor* a = 
        static_cast< tdk::io::ip::tcp::acceptor* >( r->data());
    a->close();
}


TEST( loop , accept ) {
    tdk::loop loop;
    tdk::io::ip::tcp::acceptor acceptor( loop );
    EXPECT_TRUE(acceptor.open( tdk::io::ip::address::any( 9999 ) 
                , &on_accept
                , &acceptor ));

    loop.run();
}
