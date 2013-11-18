// test.udp.echo.server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <tdk/tdk.hpp>
#include <tdk/event_loop/event_loop.hpp>
#include <tdk/io/ip/udp/pipeline/filter.hpp>
#include <tdk/io/ip/udp/bootstrap.hpp>
#include <tdk/io/ip/udp/pipeline/pipeline_builder.hpp>
#include <tdk/threading/thread_data.hpp>
#include <vector>
#include <atomic>


class echo_handler : public tdk::io::ip::udp::filter {
public:
	echo_handler( void ) {

	}
	virtual ~echo_handler( void ) {
	}

	virtual void on_open( void ) {
	}
	virtual void on_error( const std::error_code& ec ) {
		channel()->close();
	}

	virtual void on_read( const tdk::io::ip::address& addr 
		, tdk::buffer::memory_block& msg ) 
	{
		write_out_bound( addr , msg );
	}

	virtual void on_closed( void ) {

	}
};

class echo_builder : public tdk::io::ip::udp::pipeline_builder{
public:
	echo_builder( void ) {		
	}
	virtual std::error_code build( tdk::io::ip::udp::pipeline& p ) {
		p.add( "echo" , new echo_handler()  );
		return std::error_code();
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	tdk::init();

	echo_builder b;
	tdk::event_loop l;
	tdk::io::ip::udp::bootstrap bs(l);

	if ( bs.start(tdk::io::ip::address::any( 7543 ),&b)) {
		l.run();
	}
	return 0;
}

