#include "stdafx.h"
#include <tdk/event_loop/io/ip/tcp/pipeline/connector.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline_builder.hpp>
#include <tdk/log/logger.hpp>

class test_connector : public tdk::io::ip::tcp::connector {
public:
	test_connector( tdk::event_loop& l ) 
		: connector( l ) {
	}

	virtual void on_connnect( const tdk::io::ip::address& addr ) {
		LOG_D( L"test.logger" , L"connect" );
	}

	virtual void on_connect_fail( const std::error_code& ec ) {
		LOG_D( L"test.logger" , L"connect fail" );
	}
private:
};

TEST( connector , connect ) {
	tdk::event_loop loop;
	tdk::io::ip::tcp::pipeline_builder builder;
	test_connector c( loop );
	
	std::vector<tdk::io::ip::address> addrs;
	tdk::io::ip::address::resolve( "google.co.kr" , 80  , addrs , AF_INET );

	c.connect( addrs , &builder );

	loop.run();
}