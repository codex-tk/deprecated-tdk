#include "stdafx.h"
#include <tdk.task/network/udp/channel.hpp>
#include <tdk.task/task/event_loop.hpp>


void on_recv_from( tdk::network::udp::channel* client , tdk::network::udp::recvfrom_operation& r ) {
	if ( r.error() ) 
		return;

	int data;
	r.buffer().read( &data , sizeof(data));
	++data;

	if ( data > 10 ) {
		r.channel()->close();
		client->close();
		return;
	}

	tdk::buffer::memory_block mb( &data , sizeof(int) , sizeof(int),nullptr);
	r.channel()->sendto( mb , r.address() );
	r.buffer().clear();
	r.channel()->recvfrom( r.buffer() , [client] (tdk::network::udp::recvfrom_operation& r){
		on_recv_from( client , r );
	});
}


void on_client_recv_from( tdk::network::udp::recvfrom_operation& r ) {
	if ( r.error() ) 
		return;

	r.channel()->sendto( r.buffer() , r.address());
	r.buffer().clear();
	r.channel()->recvfrom( r.buffer() , &on_client_recv_from );
}

TEST( udp_channel , init ){

	tdk::task::event_loop loop;
	ASSERT_TRUE( loop.open());

	tdk::network::udp::channel svr( loop );
	tdk::network::udp::channel client( loop );

	ASSERT_TRUE(svr.open());
	ASSERT_TRUE(client.open());

	ASSERT_TRUE(svr.bind( tdk::network::address::any(9999)));
	
	svr.recvfrom( tdk::buffer::memory_block(4096), [&client] (tdk::network::udp::recvfrom_operation& r){
		on_recv_from( &client , r );
	});

	tdk::network::address send_addr( "127.0.0.1" , 9999 );

	int id = 0;
	tdk::buffer::memory_block mb( &id , sizeof(int) , sizeof(int),nullptr);

	client.sendto( mb , send_addr );
	client.recvfrom( tdk::buffer::memory_block(8) , &on_client_recv_from );
	
	loop.run();

	loop.close();

}