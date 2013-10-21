#include <gtest/gtest.h>
#include <tdk/event_loop/event_loop.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline_connector.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline_builder.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/filter.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline.hpp>

TEST( tdk_connector , connect ) {
	tdk::event_loop loop;

	class test_connector : public tdk::io::ip::tcp::pipeline_connector {
	public:
		test_connector( tdk::event_loop& l )
			: pipeline_connector( l ) {

		}

		virtual ~test_connector( void ) {

		}

		virtual void on_connect_fail( const std::error_code& ec ) {
			printf( "On Connect Fail %s\r\n" , ec.message().c_str());
		}

		virtual bool on_connnect( const tdk::io::ip::address& addr ) {
			printf( "On Connect %s\r\n" , addr.ip_address().c_str());
			return true;
		}
	private:
	};
	
	class test_filter : public tdk::io::ip::tcp::filter {
	public:
		test_filter( void ) {

		}
		virtual ~test_filter( void ) {

		}
		virtual void on_connected( void ) {
			tdk::io::ip::tcp::message msg;
			msg.data().write( "GET /index HTTP/1.1\r\n\r\n" );
			write_out_bound( msg );
		}
		virtual void on_error( const std::error_code& ec ) {
			printf( "error %s\r\n" , ec.message().c_str());
			pipeline()->close();
		}
		virtual void on_read( tdk::io::ip::tcp::message& msg ) {
			printf("On Read %d\r\n" , (int)msg.data().length());
			char end = '\0';
			msg.data().write(&end,1);
			printf("%s\r\n" , msg.data().rd_ptr());
		}

		virtual void on_closed( void ) {
			printf("On Close\r\n" );
		}
	};

	class test_builder : public tdk::io::ip::tcp::pipeline_builder{
	public:
		virtual std::error_code build( tdk::io::ip::tcp::pipeline& p ) {
			p.add( new test_filter());
			return std::error_code();
		}
	};

	test_connector connector( loop );

	std::vector< tdk::io::ip::address > addrs;
	//addrs.push_back( tdk::io::ip::address("192.168.0.2" , 12345 ) );
	tdk::io::ip::address::resolve(
			"google.co.kr"
			, 80
			, addrs
			, AF_INET );


	test_builder builder;
	connector.connect( addrs , &builder , tdk::time_span::from_seconds(10));
	loop.run();

}
