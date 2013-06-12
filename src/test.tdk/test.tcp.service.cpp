#include "stdafx.h"
#include <tdk.task/task/event_loop.hpp>
#include <tdk.task/network/tcp/acceptor.hpp>
#include <tdk.task/network/tcp/connector.hpp>
#include <tdk.task/network/tcp/channel.hpp>
#include <tdk.task/network/tcp/stream.hpp>
#include <tdk/log/logger.hpp>
#include <tdk.task/task/timer.hpp>
/*

class handler_impl : public tdk::network::tcp::stream_handler {
public:
	handler_impl( void ) {

	}

	virtual ~handler_impl( void ) {

	}

	virtual void on_recv( tdk::network::tcp::stream* s , tdk::buffer::memory_block& mb ) {
		tdk::log::logger logger( "test.logger" );
		LOG_D( logger , "OnRecv" );
		s->send( mb );
		s->recv( tdk::buffer::memory_block(4096));
		s->close();
	}

	virtual void on_send( tdk::network::tcp::stream* s , int send_bytes , int remain_bytes ) {
		tdk::log::logger logger( "test.logger" );
		LOG_D( logger , "OnSend ");
	}

	virtual void on_error( tdk::network::tcp::stream* s , const tdk::error_code& code ) {
		tdk::log::logger logger( "test.logger" );
		LOG_D( logger , "on_error %s" , code.message().c_str());
		s->close();
	}
	virtual void on_close( tdk::network::tcp::stream* s ) {
		tdk::log::logger logger( "test.logger" );
		LOG_D( logger , "on_close %s" , code.message().c_str());
		delete this;
	}
private:
};
*/

TEST( event_loop , stream ){
	/*
	tdk::task::event_loop loop;
	ASSERT_TRUE( loop.open());

	tdk::network::tcp::acceptor acceptor( loop );
	tdk::network::tcp::stream s(loop);
	acceptor.open( tdk::network::address::any( 8888 ));
	acceptor.accept( [&s]( tdk::network::tcp::accept_operation& r ) {
		s.open( r.socket() , new handler_impl );
		s.recv( tdk::buffer::memory_block(4096));
	});

	loop.run();

	loop.close();*/
}


class handler_impl2 : public tdk::network::tcp::stream_handler {
public:
	handler_impl2( void ) {

	}

	virtual ~handler_impl2( void ) {

	}

	virtual void on_recv( tdk::network::tcp::stream& s , tdk::buffer::memory_block& mb ) {
		tdk::log::logger logger( L"test.logger" );
		LOG_D( logger , "OnRecv" );

		int* header = (int*)mb.rd_ptr();
		if ( mb.length() > 16 ) {
			int type  = header[0];
			int width = header[1];
			int height= header[2];
			int size  = header[3];
			if ( mb.length() >= size + 16 ) {

				mb.rd_ptr( size + 16 );
				mb.align();
			} else {
				mb.reserve( size + 16 );
			}
		} 
		s.recv( mb );
	}

	virtual void on_send( tdk::network::tcp::stream& s , int send_bytes , int remain_bytes ) {
		/*
		tdk::log::logger logger( "test.logger" );
		LOG_D( logger , "OnSend ");
		tdk::task::timer timer(s.channel().loop());
		timer.expired_at( tdk::date_time::local() + tdk::time_span::from_seconds(2));
		timer.handler(
			[&]( tdk::error_code& e) {
				s.close();
			});
		timer.schedule();*/
	}

	virtual void on_error( tdk::network::tcp::stream& s , const tdk::error_code& code ) {
		tdk::log::logger logger( L"test.logger" );
		LOG_D( logger , "on_error %s" , code.message().c_str());
		
	}
	virtual void on_close( tdk::network::tcp::stream& s ) {
		tdk::log::logger logger( L"test.logger" );
		LOG_D( logger , "on_close" );
		delete this;
	}
private:
};


TEST( tcp_stream , connect ) {
	return;
	tdk::task::event_loop loop;
	ASSERT_TRUE( loop.open());

	tdk::network::tcp::connector connector;
	tdk::network::tcp::stream s(loop);

	connector.connect( s.channel() 
		, tdk::network::address( "127.0.0.1" , 9999 )
		, [&s] ( tdk::network::tcp::connect_operation& r ) {
			s.open( new handler_impl2());
			s.recv( tdk::buffer::memory_block(16384));
		});
	loop.run();

	loop.close();

}