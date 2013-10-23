#include "stdafx.h"
#include <tdk/io/ip/tcp/channel_connector.hpp>
#include <tdk/io/ip/tcp/pipeline/pipeline_builder.hpp>
#include <tdk/io/ip/tcp/pipeline/filter.hpp>
#include <tdk/log/logger.hpp>

class handler : public tdk::io::ip::tcp::filter {
public:
	virtual void on_connected( void ) {
		tdk::buffer::memory_block mb(256);
		mb.write( "GET /index HTTP/1.1\r\n\r\n" );
		write_out_bound( mb );
	}

	virtual void on_error( const std::error_code& ec ) {
		LOG_D( L"test.logger" , L"error" );
	}

	virtual void on_read( tdk::buffer::memory_block& msg ) {
		LOG_D( L"test.logger" , L"read" );
		channel()->close();
	}

	virtual void on_closed( void ) {
		LOG_D( L"test.logger" , L"close" );
	}
	virtual void on_delete( void ) {
		LOG_D( L"test.logger" , L"on_delete" );
	}
};

class test_connector : public tdk::io::ip::tcp::channel_connector
	, public tdk::io::ip::tcp::pipeline_builder
{
public:
	test_connector( tdk::event_loop& l ) 
		: channel_connector( l ) {
	}

	virtual bool on_connnect( const tdk::io::ip::address& addr ) {
		LOG_D( L"test.logger" , L"connect" );
		return true;
	}

	virtual void on_connect_fail( const std::error_code& ec ) {
		LOG_D( L"test.logger" , L"connect fail" );
	}

	virtual std::error_code build( tdk::io::ip::tcp::pipeline& p ) {
		p.add( new handler());
		return std::error_code();
	}
private:
};

TEST( connector , connect ) {
	tdk::event_loop loop;
	test_connector c( loop );
	
	std::vector<tdk::io::ip::address> addrs;
	tdk::io::ip::address::resolve( "google.co.kr" , 80  , addrs , AF_INET );
	c.connect( addrs , &c );
	loop.run();
}
