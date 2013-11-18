//============================================================================
// Name        : echo.cpp
// Author      : tk
// Version     :
// Copyright   : tdk
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <iostream>
using namespace std;

#include <tdk/tdk.hpp>
#include <tdk/event_loop/event_loop.hpp>
#include <tdk/io/ip/tcp/channel_acceptor.hpp>
#include <tdk/io/ip/tcp/channel.hpp>
#include <tdk/buffer/memory_block.hpp>
#include <memory>
#include <tdk/event_loop/event_loop.hpp>
#include <tdk/io/ip/tcp/channel_acceptor.hpp>
#include <tdk/io/ip/tcp/pipeline/pipeline_builder.hpp>
#include <tdk/io/ip/tcp/channel_connector.hpp>
#include <tdk/io/ip/tcp/pipeline/filter.hpp>
#include <thread>
#include <system_error>

class echo_handler : public tdk::io::ip::tcp::filter {
public:
	echo_handler( void ) {

	}
	virtual ~echo_handler( void ) {
		printf("deleted!!\n");
	}/*
	virtual void on_connected( void ) {
		tdk::io::ip::tcp::message msg;
		msg.data().write( "GET /index HTTP/1.1\r\n\r\n" );
		pipeline()->write( msg );
		//write_out_bound( msg );
	}*/


	virtual void on_accepted( const tdk::io::ip::address& addr ) {
		printf( "accept %s\n" , addr.ip_address().c_str());
	}
	virtual void on_error( const std::error_code& ec ) {
		printf( "error %s\n" , ec.message().c_str());
		channel()->close();
	}

	virtual void on_read( tdk::buffer::memory_block& msg ) {
		printf("On Read %d\n" , (int)msg.length());
		write_out_bound( msg );
	}

	virtual void on_write( int w , bool flushed ) {
		printf( "Write %d\n" , w);
	}

	virtual void on_closed( void ) {
		printf("On Close\r\n" );
	}
};


class echo_builder : public tdk::io::ip::tcp::pipeline_builder{
public:
	virtual std::error_code build( tdk::io::ip::tcp::pipeline& p ) {
		p.add( "" , new echo_handler());
		return std::error_code();
	}
};



int main() {

	tdk::init();

	tdk::event_loop l;
	echo_builder b;

	tdk::io::ip::tcp::channel_acceptor a(l);
	if ( a.open(tdk::io::ip::address::any( 9999 ) , &b )) {
		l.run();
	}
}
/*
typedef std::shared_ptr< tdk::io::ip::tcp::channel > channel_ptr;

void on_write( channel_ptr p , tdk::buffer::memory_block mb
		, tdk::io::ip::tcp::write_task* wt );
void on_read( channel_ptr p , tdk::buffer::memory_block mb
		, tdk::io::ip::tcp::read_task* rt )
{
	if ( rt->error() ) {
		printf( "Read Error %s\r\n" , rt->error().message().c_str() );
		p->close( [p]{});

	} else {
		mb.wr_ptr( rt->io_bytes());
		p->write( tdk::io::buffer_adapter( mb.rd_ptr() , mb.length() )
			, [p,mb](tdk::io::ip::tcp::write_task* wt ){
				on_write( p , mb , wt );
		} );
	}
}

void on_write( channel_ptr p , tdk::buffer::memory_block mb
		, tdk::io::ip::tcp::write_task* wt )
{

	if ( wt->error() ) {
		printf( "Write Error %s\r\n" , wt->error().message().c_str() );
		p->close( [p]{});
	} else {
		mb.clear();
		p->read( tdk::io::buffer_adapter(mb.wr_ptr() , mb.space())
		, [p,mb]( tdk::io::ip::tcp::read_task* rt ){
			on_read( p , mb , rt );
		});
	}
}
int main() {


	tdk::init();

	tdk::event_loop loop;
	tdk::io::ip::tcp::acceptor acceptor( loop );

	tdk::handler_task< tdk::io::ip::tcp::accept_task , void > accept_task_instance(
				[&loop]( tdk::io::ip::tcp::accept_task* at ) {
					if ( at->error() ) {

					} else {
						channel_ptr ptr( new tdk::io::ip::tcp::channel(loop));
						if ( ptr->accept( at )) {
							tdk::buffer::memory_block mb(4096);
							ptr->read( tdk::io::buffer_adapter(mb.wr_ptr() , mb.space())
								, [ptr,mb]( tdk::io::ip::tcp::read_task* rt ){
									on_read( ptr , mb , rt );
								});
						}
					}
				});

	acceptor.open(
				tdk::io::ip::address::any( 9999 , AF_INET )
				, &accept_task_instance
			);


	loop.run();
	return 0;
}*/
