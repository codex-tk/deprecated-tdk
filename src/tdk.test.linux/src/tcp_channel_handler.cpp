/*
 * tcp_channel_handler.cpp
 *
 *  Created on: 2013. 10. 15.
 *      Author: tk
 */



#include <gtest/gtest.h>
#include <tdk/event_loop/io/ip/tcp/channel.hpp>
#include <tdk/event_loop/event_loop.hpp>

void on_close(  ){
	printf( "Closed!!\r\n");
}

void on_read( tdk::io::ip::tcp::read_task* rt ){
	delete [] (char*)(rt->buffers().buffers()[0].iov_base);
	if ( rt->error()) {
		printf( "Read Error %s\r\n" , rt->error().message().c_str());
	} else {
		printf( "Read %d \r\n" , rt->io_bytes());
		if ( rt->io_bytes() != 256 ) {
			rt->channel()->close( []{
				on_close();
			});
		} else {
			rt->channel()->read(
					tdk::io::buffer_adapter( new char[4096] , 256 )
				,	[]( tdk::io::ip::tcp::read_task* rt  ) {
						on_read( rt );
					}
				);
		}
	}
}


void on_write( tdk::io::ip::tcp::write_task* wt  ){

	if ( wt->error()) {
		printf( "Write Error %s\r\n" , wt->error().message().c_str());
		return;
	} else {
		printf( "Write Success\r\n" );
	}

	wt->channel()->read(
				tdk::io::buffer_adapter( new char[4096] , 256 )
			,	[]( tdk::io::ip::tcp::read_task* rt  ) {
					on_read( rt );
				}
			);
}


void on_connect( tdk::io::ip::tcp::connect_task* ct ) {
	if ( ct->error() ) {
		printf( "Connect Error %s\r\n" , ct->error().message().c_str());
	} else {
		printf( "Connect Success\r\n" );
		char* buf = new char[1024];
		int len = sprintf( buf , "GET /index HTTP/1.1\r\n\r\n");
		tdk::io::buffer_adapter buf_adpt( buf , len );
		ct->channel()->write(buf_adpt ,
				[] ( tdk::io::ip::tcp::write_task* wt ) {
					on_write( wt );
				}
			);
	}
}
TEST( tcp__channel_handler , init ) {
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

	chan.connect(ipv4_addrs , []( tdk::io::ip::tcp::connect_task* ct ){
			on_connect(ct);
		});
	tdk::event_loop::default_loop().run();
}




