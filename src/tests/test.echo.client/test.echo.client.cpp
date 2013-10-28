// test.echo.client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <tdk/tdk.hpp>
#include <tdk/io/ip/tcp/pipeline/filter.hpp>
#include <tdk/io/ip/tcp/pipeline/pipeline_builder.hpp>
#include <tdk/io/ip/tcp/filters/recv_time_out_filter.hpp>
#include <tdk/io/ip/tcp/channel_connector.hpp>
#include <tdk/threading/thread_data.hpp>
#include <vector>
#include <atomic>

std::atomic< int > connected;
std::atomic< int > connect_failed;
std::atomic< int > fail;
std::atomic< int > succ;

class handler : public tdk::io::ip::tcp::filter {
public:
	virtual void on_connected( void ) {
		++connected;
		_id = 0;
		tdk::buffer::memory_block mb(4);
		mb.write( &_id , 4 );
		write_out_bound( mb );
	}

	virtual void on_error( const std::error_code& ec ) {
		channel()->close();
	}

	virtual void on_read( tdk::buffer::memory_block& msg ) {
		int read_id;
		msg.read( &read_id , 4 );
		if ( read_id != _id ) {
			++fail;
			channel()->close();
			return;
		}
		++_id;
		if ( _id > 65535 ) {
			++succ;
			channel()->close();
			return;
		}
		tdk::buffer::memory_block mb(4);
		mb.write( &_id , 4 );
		write_out_bound( mb );
	}
	virtual void on_closed( void ) {

	}
private:
	int _id;
};

class test_connector : public tdk::io::ip::tcp::channel_connector
	, public tdk::io::ip::tcp::pipeline_builder
{
public:
	test_connector( tdk::event_loop& l ) 
		: channel_connector( l ) 
	{
		
	}

	virtual bool on_connnect( const tdk::io::ip::address& addr ) {
		return true;
	}

	virtual void on_connect_fail( const std::error_code& ec ) {
		++connect_failed;
	}

	virtual std::error_code build( tdk::io::ip::tcp::pipeline& p ) {
		p.add( "" ,  new handler());
		return std::error_code();
	}	
};



int _tmain(int argc, _TCHAR* argv[])
{

	connected.store(0);
	connect_failed.store(0);
	fail.store(0);
	succ.store(0);

	tdk::init();

	tdk::event_loop loop;

	tdk::date_time begin = tdk::date_time::utc();

	std::vector< test_connector* > connectors;
	for ( int i = 0 ; i < 10 ; ++i ) {
		test_connector* c = new test_connector( loop );
		std::vector<tdk::io::ip::address> addrs;
		addrs.push_back( 
			tdk::io::ip::address( "127.0.0.1" , 7543 ));
		c->connect( addrs , c );
		connectors.push_back(c);
	}	
	
	loop.run();

	printf( "%d %d %d %d %d\n" 
		, ( tdk::date_time::utc() - begin ).total_seconds()
		, connected.load()
		, connect_failed.load()
		, fail.load()
		, succ.load());
	return 0;
}

