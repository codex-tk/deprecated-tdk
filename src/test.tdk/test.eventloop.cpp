#include "stdafx.h"
#include <tdk.task/task/event_loop.hpp>
#include <tdk.task/network/tcp/acceptor.hpp>
#include <tdk/log/logger.hpp>

TEST( event_loop , init ){
	tdk::task::event_loop loop;

	ASSERT_TRUE( loop.open());

	loop.close();

}

tdk::network::tcp::channel* chan;

void __stdcall on_accept_complete( const tdk::error_code& code
		, tdk::network::tcp::acceptor& acceptor
		, tdk::network::socket fd 
		, void* ctx )
{
	if ( chan->open( fd ) ) {
		chan->recv( tdk::buffer::memory_block(4096));
	}
	acceptor.accept();
}

void __stdcall on_recv_complete( const tdk::error_code& code
		, tdk::network::tcp::channel& channel
		, tdk::buffer::memory_block& mb
		, void* ctx )
{
	if ( code ) {

	} else {
		char e = '\0';
		mb.write( &e , 1);
		tdk::log::logger log("test.logger");
		LOG_D( log , "%s" , mb.rd_ptr());
		mb.clear();
		channel.recv( mb );
	}
}


TEST( event_loop , acceptor ) {
	tdk::task::event_loop loop;
	ASSERT_TRUE( loop.open());

	loop.set_on_accept( &on_accept_complete , nullptr );
	loop.set_on_recv( &on_recv_complete , nullptr );

	tdk::network::tcp::acceptor acceptor( loop );
	tdk::network::tcp::channel channel( loop );
	chan = &channel;
	ASSERT_TRUE( acceptor.open( tdk::network::address::any( 7543 )));

	acceptor.accept();

	loop.run_once();
	loop.run_once();
	loop.run_once();
	loop.run_once();
	acceptor.close();
	loop.close();	
}

