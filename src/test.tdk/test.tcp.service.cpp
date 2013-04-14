#include "stdafx.h"
#include <tdk.task/task/event_loop.hpp>
#include <tdk.task/network/tcp/acceptor.hpp>
#include <tdk.task/network/tcp/connector.hpp>
#include <tdk.task/network/tcp/channel.hpp>
#include <tdk.task/network/tcp/stream.hpp>
#include <tdk/log/logger.hpp>


class handler_impl : public tdk::network::tcp::stream_handler {
public:
	handler_impl( void ) {

	}

	virtual ~handler_impl( void ) {

	}

	void on_recv( tdk::network::tcp::stream* s , tdk::buffer::memory_block& mb ) {
		tdk::log::logger logger( "test.logger" );
		LOG_D( logger , "OnRecv" );
		s->send( mb );
		s->recv( tdk::buffer::memory_block(4096));
		s->close();
	}

	void on_send( tdk::network::tcp::stream* s , int send_bytes , int remain_bytes ) {
		tdk::log::logger logger( "test.logger" );
		LOG_D( logger , "OnSend ");
	}

	void on_close( tdk::network::tcp::stream* s , const tdk::error_code& code ) {
		tdk::log::logger logger( "test.logger" );
		LOG_D( logger , "OnClose %s" , code.message().c_str());
		delete this;
	}
private:
};


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

	void on_recv( tdk::network::tcp::stream* s , tdk::buffer::memory_block& mb ) {
		tdk::log::logger logger( "test.logger" );
		LOG_D( logger , "OnRecv" );
		s->close();
	}

	void on_send( tdk::network::tcp::stream* s , int send_bytes , int remain_bytes ) {
		tdk::log::logger logger( "test.logger" );
		LOG_D( logger , "OnSend ");
	}

	void on_close( tdk::network::tcp::stream* s , const tdk::error_code& code ) {
		tdk::log::logger logger( "test.logger" );
		LOG_D( logger , "OnClose %s" , code.message().c_str());
		delete this;
	}
private:
};


TEST( tcp_stream , connect ) {

	tdk::task::event_loop loop;
	ASSERT_TRUE( loop.open());

	tdk::network::tcp::connector connector;
	tdk::network::tcp::stream s(loop);

	connector.connect( &s.channel() 
		, tdk::network::address( "google.co.kr" , 80 )
		, [&s] ( tdk::network::tcp::connect_operation& r ) {
			s.open( new handler_impl2());
			s.recv( tdk::buffer::memory_block(16384));
			
			tdk::buffer::memory_block request(4096);
			request.write( "GET /index HTTP/1.1\r\n\r\n" );
			s.send( request );
		});

	/*
	acceptor.open( tdk::network::address::any( 8888 ));
	acceptor.accept( [&s]( tdk::network::tcp::accept_operation& r ) {
		s.open( r.socket() , new handler_impl );
		s.recv( tdk::buffer::memory_block(4096));
	});
	*/
	loop.run();

	loop.close();

}