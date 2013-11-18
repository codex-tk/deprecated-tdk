#include <cstdio>
#include <tdk/tdk.hpp>
#include <tdk/log/logger.hpp>
#include <tdk/log/writer/stderr_writer.hpp>
#include <sys/epoll.h>
#include <tdk/error/error_platform.hpp>
#include <tdk/io/eventfd.hpp>
#include <tdk/io/ip/socket.hpp>
#include <tdk/io/timerfd.hpp>
#include <tdk/loop/loop.hpp>
#include <tdk/io/ip/tcp/acceptor_linux.hpp>
#include <tdk/io/ip/tcp/channel_linux.hpp>
#include <tdk/buffer/memory_block.hpp>
#include <tdk/event_loop/event_loop.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline_acceptor.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline_builder.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/filter.hpp>
#include <thread>

class echo_handler : public tdk::io::ip::tcp::filter {
public:
	echo_handler( void ) {

	}
	virtual ~echo_handler( void ) {

	}/*
	virtual void on_connected( void ) {
		tdk::io::ip::tcp::message msg;
		msg.data().write( "GET /index HTTP/1.1\r\n\r\n" );
		pipeline()->write( msg );
		//write_out_bound( msg );
	}*/

	virtual void on_accepted( const tdk::io::ip::address& addr ) {
		printf( "accept %s\r\n" , addr.ip_address().c_str());
	}
	virtual void on_error( const std::error_code& ec ) {
		printf( "error %s\r\n" , ec.message().c_str());
		pipeline()->close();
	}
	virtual void on_read( tdk::io::ip::tcp::message& msg ) {
		printf("On Read %d\r\n" , (int)msg.data().length());
		write_out_bound( msg );
	}

	virtual void on_write( int w , bool flushed ) {
		printf( "Write %d\r\n" , w);
	}

	virtual void on_closed( void ) {
		printf("On Close\r\n" );
	}
};


class echo_builder : public tdk::io::ip::tcp::pipeline_builder{
public:
	virtual std::error_code build( tdk::io::ip::tcp::pipeline& p ) {
		p.add( new echo_handler());
		return std::error_code();
	}
};
/*
void on_accept( tdk::req_handle* arg );
void on_close( tdk::req_handle* arg );

class channel_impl {
public:
    channel_impl( void )
        : _channel( tdk::loop::default_loop())            
        , _close( false )
    {
        _close_req.set( ::on_close , this );
    }

    ~channel_impl( void ){
    }

    void open( int fd ) {
        _channel.open(fd);
        read();
    }

    void read( void ){
        tdk::buffer::memory_block mb(4096);
        tdk::io::buffer_adapter adapter( mb.rd_ptr(), 4096 );
        _channel.async_read( adapter ,
                [ this , mb ]( tdk::io::ip::tcp::read_req& req ){
                    tdk::buffer::memory_block mb2( mb );
                    mb2.wr_ptr( req.io_bytes());
                    on_read( req , mb2 );
                });
    }

    void close( void ) {
        if ( !_close )
            _channel.async_close( &_close_req );
        _close = false;
    }

    void on_read( tdk::io::ip::tcp::read_req& req , 
            tdk::buffer::memory_block& read_buffer ) {
        if ( req.error() ) {
            printf( "error\r\n" );
            _channel.async_close( &_close_req );
        } else {
            if ( req.io_bytes() == 0 ) {
                 printf( "error\r\n" );
                _channel.async_close( &_close_req );
            } else {
                read();
                _channel.async_write( 
                    tdk::io::buffer_adapter( 
                        read_buffer.rd_ptr()
                        , read_buffer.length())
                    , [this,read_buffer]( tdk::io::ip::tcp::write_req& req){
                        on_write( req );
                    });

            }
        }
    }

    void on_write( tdk::io::ip::tcp::write_req& req ) {
        if ( req.error()){
            _channel.async_close( &_close_req  );
        }
    }

    void on_close( void ) {

    }
private:
    tdk::io::ip::tcp::channel _channel;
    tdk::io::ip::tcp::close_req _close_req;
    bool _close;
};
*/
int main( int argc , char* argv[] ) {
    tdk::init();
    tdk::log::logger logger = tdk::log::logger::get_instance("echo");
    tdk::log::writer_ptr ptr = tdk::log::stderr_writer::instance();
    logger.add_writer( ptr );
    LOG_D( "echo", "test %s" , "test1" );

    tdk::event_loop l;
    echo_builder b;

    tdk::io::ip::tcp::pipeline_acceptor a(l);
    if ( a.open(tdk::io::ip::address::any( 9999 ) , &b )) {
    	l.run();
    }

/*
    tdk::loop& main_loop = tdk::loop::default_loop();

    tdk::io::ip::tcp::acceptor acceptor( main_loop );

    if ( acceptor.open( tdk::io::ip::address::any( 9999 )
                , on_accept
                , nullptr ))
    {
        main_loop.run();
    }
*/
    return 0;
}/*

void on_accept( tdk::req_handle* arg ) {
    tdk::io::ip::tcp::accept_req* req = 
        static_cast< tdk::io::ip::tcp::accept_req*>( arg );
    if ( req->error() ) {
        
    } else {
        channel_impl* impl = new channel_impl();
        impl->open( req->accepted_fd());
    }
}
void on_close( tdk::req_handle* arg ) {
    tdk::io::ip::tcp::close_req* req =
        static_cast< tdk::io::ip::tcp::close_req* >( arg );
    channel_impl* impl = static_cast< channel_impl* >( req->data());
    impl->on_close();

}*/
