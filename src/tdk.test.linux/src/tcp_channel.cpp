/*
 * tcp_channel.cpp
 *
 *  Created on: 2013. 10. 10.
 *      Author: tk
 */

#include <gtest/gtest.h>
#include <tdk/event_loop/io/ip/tcp/channel.hpp>
#include <tdk/event_loop/event_loop.hpp>

static void test_on_close( tdk::task* t ){
	tdk::io::ip::tcp::close_task* ct
		= static_cast<tdk::io::ip::tcp::close_task*>(t);
	delete ct;
	printf( "Closed!!\r\n");
}

static void test_on_read( tdk::task* t ){
	tdk::io::ip::tcp::read_task* impl =
				static_cast<tdk::io::ip::tcp::read_task*>(t);
	delete [] (char*)(impl->buffers().buffers()[0].iov_base);
	if ( impl->error()) {
		printf( "Read Error %s\r\n" , impl->error().message().c_str());
	} else {
		printf( "Read %d \r\n" , impl->io_bytes());
		if ( impl->io_bytes() != 256 ) {
			impl->channel()->close( new tdk::io::ip::tcp::close_task( &test_on_close , nullptr ));
			delete impl;
		} else {
			impl->channel()->read( new char[4096] , 256 , impl );
		}
	}
}

static void test_on_write( tdk::task* t ){
	tdk::io::ip::tcp::write_task* impl = static_cast<
			tdk::io::ip::tcp::write_task* >( t );
	tdk::io::ip::tcp::read_task* rt =
					new tdk::io::ip::tcp::read_task( &test_on_read , nullptr );
	impl->channel()->read( new char[4096] , 256 , rt );
	if ( impl->error() ) {
		printf( "Write Error %s\r\n" , impl->error().message().c_str());
	} else {
		printf( "Write Success\r\n" );
	}
}

static void test_connect_handler ( tdk::task* ct ) {
	tdk::io::ip::tcp::connect_task* impl =
			static_cast<tdk::io::ip::tcp::connect_task*>(ct);
	if ( impl->error() ) {
		printf( "Connect Error %s\r\n" , impl->error().message().c_str());
	} else {
		printf( "Connect Success\r\n" );
		char* buf = new char[1024];
		int len = sprintf( buf , "GET /index HTTP/1.1\r\n\r\n");
		tdk::io::ip::tcp::write_task* wt
			= new tdk::io::ip::tcp::write_task( &test_on_write , nullptr);
		impl->channel()->write( buf , len , wt );
		/*socket().send( (void*)
				, strlen( "GET /index HTTP/1.1\r\n\r\n" )
				, 0 );*/
	}

}

TEST( tcp__channel , init ) {
	tdk::io::ip::tcp::channel chan(tdk::event_loop::default_loop());
	std::vector<tdk::io::ip::address> addrs;
	tdk::io::ip::address::resolve(
			"google.co.kr" , 80 , addrs
			);

	std::vector< tdk::io::ip::address > ipv4_addrs;
	for( auto it : addrs ) {
		if ( it.family() == AF_INET )
			ipv4_addrs.push_back(it);
	}
	tdk::io::ip::tcp::connect_task ct;
	ct.set_handler( &test_connect_handler , nullptr );
	chan.connect(ipv4_addrs , &ct );
	tdk::event_loop::default_loop().run();
}


