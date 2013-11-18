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
		_cnt = 0;
	}
	virtual ~echo_handler( void ) {
	}

	void write( void ) {
		tdk::buffer::memory_block mb(4);
		mb.write(&_cnt,4);
		tdk::io::ip::address addr( "127.0.0.1" , 7543 );
		write_out_bound(addr,mb);
		++_cnt;
		if ( _cnt > 100 ) 
			channel()->close();
	}

	virtual void on_open( void ) {
		std::cout << "Open" << std::endl;
		write();
	}
	virtual void on_error( const std::error_code& ec ) {
		std::cout << "Error" << std::endl;
		channel()->close();
	}

	virtual void on_read( const tdk::io::ip::address& addr 
		, tdk::buffer::memory_block& msg ) 
	{
		std::cout << "Read" << std::endl;
		write();
	}

	virtual void on_closed( void ) {
		std::cout << "Close" << std::endl;
	}
private:
	int _cnt;
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

	if ( bs.start( AF_INET ,&b)) {
		l.run();
	}
	getchar();
	return 0;
}

