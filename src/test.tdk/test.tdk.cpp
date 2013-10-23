// test.tdk.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <tdk/tdk.hpp>
#include <thread>
#include <tdk/log/logger.hpp>
#include <tdk/error/error_platform.hpp>
#include <tdk/log/writer/console_writer.hpp>
#if defined ( _DEBUG )
	#pragma comment( lib , "gtest_x86_debug_mtdd")
	//#pragma comment( lib , "gtest_x86_debug_mtd")
#else
	#pragma comment( lib , "gtest_x86_release_mt")
	//#pragma comment( lib , "gtest_x86_release_md")
#endif

#pragma comment( lib , "tdk.lib")
//#pragma comment( lib , "tdk.adodb.lib")
#pragma comment( lib , "ws2_32")
#pragma comment( lib , "mswsock")

#include <iostream>
#include <string>
#include <atomic>
#include <gtest/gtest.h>


/*
int _tmain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest( &argc , argv );
	
	std::error_code ec( tdk::platform::error() );

	std::string msg = ec.message();

	tdk::init();

	tdk::log::logger logger(L"test.logger");
	tdk::log::writer_ptr ptr = tdk::log::console_writer::instance();
	logger.add_writer( ptr );
	return RUN_ALL_TESTS();
}
*/

#include <tdk/tdk.hpp>
#include <tdk/event_loop/event_loop.hpp>
#include <tdk/io/ip/tcp/channel_acceptor.hpp>
#include <tdk/io/ip/tcp/channel.hpp>
#include <tdk/buffer/memory_block.hpp>
#include <memory>
#include <tdk/event_loop/event_loop.hpp>
#include <tdk/io/ip/tcp/channel_acceptor.hpp>
#include <tdk/io/ip/tcp/pipeline/pipeline_builder.hpp>
#include <tdk/io/ip/tcp/channel_connector.hpp>
#include <tdk/io/ip/tcp/pipeline/filter.hpp>
#include <thread>
#include <system_error>

class echo_handler : public tdk::io::ip::tcp::filter {
public:
	echo_handler( void ) {

	}
	virtual ~echo_handler( void ) {
		printf("deleted!!\n");
	}/*
	virtual void on_connected( void ) {
		tdk::io::ip::tcp::message msg;
		msg.data().write( "GET /index HTTP/1.1\r\n\r\n" );
		pipeline()->write( msg );
		//write_out_bound( msg );
	}*/


	virtual void on_accepted( const tdk::io::ip::address& addr ) {
		printf( "accept %s\n" , addr.ip_address().c_str());
	}
	virtual void on_error( const std::error_code& ec ) {
		printf( "error %s\n" , ec.message().c_str());
		channel()->close();
	}

	virtual void on_read( tdk::buffer::memory_block& msg ) {
		printf("On Read %d\n" , (int)msg.length());
		write_out_bound( msg );
	}

	virtual void on_write( int w , bool flushed ) {
		printf( "Write %d\n" , w);
	}

	virtual void on_closed( void ) {
		printf("On Close\r\n" );
	}
};


class echo_builder : public tdk::io::ip::tcp::pipeline_builder{
public:
	virtual std::error_code build( tdk::io::ip::tcp::pipeline& p ) {
		p.add( new echo_handler());
		return std::error_code();
	}
};

int main() {

	tdk::init();

	tdk::event_loop l;
	echo_builder b;

	tdk::io::ip::tcp::channel_acceptor a(l);
	if ( a.open(tdk::io::ip::address::any( 7543 ) , &b )) {
		l.run();
	}
}
	/*
	std::vector< std::thread* > thread;
	for ( int i = 0 ; i < 10 ; ++i ) {
		std::thread* t = new std::thread(
			[&]{
				tdk::network::socket fd;
				if ( fd.open_tcp( AF_INET )) {
					LOG_D( log , "Open TCP" );
					if ( fd.connect_time_out( tdk::network::address( "192.168.0.3" , 9999 )
						, 1000 ))
					{
						LOG_D( log , "Connect" );
						char msg[] = "aaa";
						while( fd.send_time_out( msg , 3 , 2000 )){
							LOG_D( log , "Send" );
							char buf[1024];
							if ( !fd.recv_time_out( buf , 3 , 2000 )){
								LOG_D( log , "Recv Fail" );
								break;
							}
							LOG_D( log , "Recv" );
							Sleep(1000);
						}
					}
				}
				LOG_E( log , "End" );
		});
	}
	getchar();
	
	*/
	//CoInitialize( nullptr );
	/*
	

	//tdk::log::logger log(L"test.logger");
	//log.add_writer( tdk::log::console_writer::instance() );
	/*
	LOG_D( log , _T("한글 %d") , 1 );	
	LOG_D( log , "%s" , tdk::platform::error(0).message().c_str());
	LOG_D( log , _T("%s") , tdk::platform::error(0).message().c_str() );
	tdk::network::address addr("google.co.kr" , 80 );
	LOG_D( log , "%s"  , addr.ip_address().c_str());
	*/
	
	/*
	// default C
	std::string old_locale = setlocale( LC_ALL , nullptr );
	// 한글에선 cp949
	old_locale = setlocale( LC_ALL , "" );
	//std::cout << tdk::string::utf_8_to_mbs( tdk::platform::error(0).message() )<< std::endl;
	*/
	//std::wcout << tdk::string::utf_8_to_wcs( tdk::platform::error( 0 ).message() ) << std::endl;
	
	//getchar();
	
//}

/*

// windows 에는 없는 로케일
//std::locale::global(std::locale("ko_KR.UTF-8")); 

utf-8 출력 로직
SetConsoleOutputCP( CP_UTF8 );
DWORD d;
WriteFile(  GetStdHandle(STD_OUTPUT_HANDLE)
, ec.message().c_str()
, ec.message().length()
, &d
, nullptr);*/

