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
#include <tdk/event_loop/io/ip/tcp/acceptor.hpp>
#include <tdk/event_loop/io/ip/tcp/channel.hpp>
#include <tdk/buffer/memory_block.hpp>
#include <memory>

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
}
