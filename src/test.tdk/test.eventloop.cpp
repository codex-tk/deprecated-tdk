#include "stdafx.h"
#include <tdk.task/task/event_loop.hpp>
#include <tdk.task/network/tcp/acceptor.hpp>
#include <tdk.task/network/tcp/connector.hpp>
#include <tdk.task/network/tcp/channel.hpp>
#include <tdk/log/logger.hpp>

TEST( event_loop , init ){
	tdk::task::event_loop loop;

	ASSERT_TRUE( loop.open());

	loop.close();
}


tdk::network::tcp::channel* chan;

tdk::network::tcp::acceptor* pacceptor;
void __stdcall on_recv_complete( tdk::network::tcp::recv_operation& r );

void __stdcall on_accept_complete( 
		tdk::network::tcp::accept_operation& op
		)
{
	if ( op.error() ) {
		return;
	}
	if ( chan->open( op.socket() ) ) {
		chan->recv( tdk::buffer::memory_block(10)
			, []( tdk::network::tcp::recv_operation& rr ) {
				on_recv_complete( rr );
			});
	}
	op.acceptor()->accept(
		[]( tdk::network::tcp::accept_operation& op ) {
			on_accept_complete( op );
		});
}

void __stdcall on_recv_complete( tdk::network::tcp::recv_operation& r )
{
	if ( r.error() || r.io_byte() == 0 ) {
		pacceptor->close();
		r.channel()->close();
	} else {
		char e = '\0';
		r.buffer().write( &e , 1);
		tdk::log::logger log("test.logger");
		LOG_D( log , "%s" , r.buffer().rd_ptr());
		r.channel()->send( r.buffer() , [] ( tdk::network::tcp::send_operation& sr ) {
		});
		r.channel()->recv( tdk::buffer::memory_block(10)
			, []( tdk::network::tcp::recv_operation& r ) {
				on_recv_complete( r );
			});
	}
}


TEST( event_loop , acceptor ) {
	tdk::task::event_loop loop;
	ASSERT_TRUE( loop.open());

	tdk::network::tcp::acceptor acceptor( loop );
	tdk::network::tcp::channel channel( loop );

	pacceptor = &acceptor;
	chan = &channel;

	ASSERT_TRUE( acceptor.open( tdk::network::address::any( 7543 )));

	tdk::network::tcp::channel conn( loop );
	tdk::network::tcp::connector connector;
	connector.connect( &conn , tdk::network::address( "127.0.0.1" , 7543 )
		, []( tdk::network::tcp::connect_operation& r ) {
			tdk::buffer::memory_block mb;
			mb.write( "test", 4 );
			r.channel()->send( mb , [] ( tdk::network::tcp::send_operation& r ) {
				int io_byte = r.io_byte();
			});
			mb.reserve(6);
			mb.clear();
			mb.write( "test2" , 5 );
			r.channel()->send( mb , [] ( tdk::network::tcp::send_operation& r ) {
				int io_byte = r.io_byte();
			});

			mb.reserve(6);
			mb.clear();
			mb.write( "test3" , 5 );
			r.channel()->send( mb , [] ( tdk::network::tcp::send_operation& r ) {
				int io_byte = r.io_byte();
			});

			mb.reserve(6);
			mb.clear();
			mb.write( "test4" , 5 );
			r.channel()->send( mb , [] ( tdk::network::tcp::send_operation& r ) {
				int io_byte = r.io_byte();
				r.channel()->close();
			});
		});

	acceptor.accept( 
		[]( tdk::network::tcp::accept_operation& op ) {
			on_accept_complete( op );
		});

	loop.run();
	
	loop.close();	
}

