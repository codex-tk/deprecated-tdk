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
#include <tdk/ssl/open_ssl.hpp>
#include <tdk/ssl/context.hpp>
#include <tdk/ssl/filter.hpp>
#include <tdk/io/ip/tcp/filters/recv_time_out_filter.hpp>


#pragma comment ( lib , "ssleay32" )
#pragma comment ( lib , "libeay32" )


class handler : public tdk::io::ip::tcp::filter {
public:
	virtual void on_connected( void ) {
		tdk::ssl::filter* f = static_cast<tdk::ssl::filter*>(
			channel()->pipeline().find("ssl"));
		X509* cert = f->peer_certificate();
		if ( cert  ) {

			f->free_peer_certificate( cert );
		}
		tdk::buffer::memory_block mb(256);
		mb.write( "GET /index HTTP/1.1\r\n\r\n" );
		write_out_bound( mb );
	}

	virtual void on_accepted( const tdk::io::ip::address& addr ) {
		LOG_D( L"test.logger" , "accept %s\n" , addr.ip_address().c_str());
	}

	virtual void on_error( const std::error_code& ec ) {
		LOG_D( L"test.logger" , L"error" );
		channel()->close();
	}

	virtual void on_read( tdk::buffer::memory_block& msg ) {
		LOG_D( L"test.logger" , L"read" );
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
	test_connector( tdk::event_loop& l , tdk::ssl::context* c ) 
		: channel_connector( l ) 
		, _ctx(c)
	{
	}

	virtual bool on_connnect( const tdk::io::ip::address& addr ) {
		LOG_D( L"test.logger" , L"connect" );
		return true;
	}

	virtual void on_connect_fail( const std::error_code& ec ) {
		LOG_D( L"test.logger" , L"connect fail" );
	}

	virtual std::error_code build( tdk::io::ip::tcp::pipeline& p ) {
		p.add( new tdk::io::ip::tcp::recv_time_out_filter(
			tdk::time_span::from_seconds(60)) , "timeout");
		p.add( new tdk::ssl::filter(_ctx->impl()) , "ssl");
		p.add( new handler() , "" );
		return std::error_code();
	}
private:
	tdk::ssl::context* _ctx;
};


class echo_handler : public tdk::io::ip::tcp::filter {
public:
	echo_handler( void ) {

	}
	virtual ~echo_handler( void ) {
		printf("deleted!!\n");
	}
	virtual bool on_connnect( const tdk::io::ip::address& addr ) {
		printf( "connecdt\n");
		tdk::buffer::memory_block mb(1024);
		return true;
	}

	virtual void on_accepted( const tdk::io::ip::address& addr ) {
		printf( "accept %s\n" , addr.ip_address().c_str());
		tdk::ssl::filter* f = static_cast<tdk::ssl::filter*>(
			channel()->pipeline().find("ssl"));
		X509* cert = f->peer_certificate();
		if ( cert  ) {

			f->free_peer_certificate( cert );
		}
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


int pem_password(char *buf, int size, int rwflag, void *userdata) {
	//strncpy_s( buf , "1331" , size );
	strncpy(buf, "1331",size);
	buf[size - 1] = '\0';
	return(strlen(buf));
}

class echo_builder : public tdk::io::ip::tcp::pipeline_builder{
public:
	echo_builder( tdk::ssl::context* ctx ) 
		: _ctx(ctx)
	{		
	}
	virtual std::error_code build( tdk::io::ip::tcp::pipeline& p ) {
		p.add( new tdk::io::ip::tcp::recv_time_out_filter(
			tdk::time_span::from_seconds(60)) , "timeout");
		p.add( new tdk::ssl::filter(_ctx->impl()) , "ssl" );
		p.add( new echo_handler() , "" );
		return std::error_code();
	}
private:
	tdk::ssl::context* _ctx;
};


int main() {
	tdk::init();

	tdk::ssl::open_ssl::init();
	tdk::ssl::context context;
	
	tdk::event_loop l;
#if defined( _SERVER )	
	if (!context.use_certificate_file("crt.pem")){
		return;
	}
	if ( !context.use_private_key_file("key.pem" , "1331")){
		return;
	}
	if ( !context.check_private_key() ) {
		return;
	}

	echo_builder b(&context);

	tdk::io::ip::tcp::channel_acceptor a(l);
	if ( a.open(tdk::io::ip::address::any( 7543 ) , &b )) {
		l.run();
	}
#else
	test_connector c( l , &context );

	std::vector<tdk::io::ip::address> addrs;
	//addrs.push_back( tdk::io::ip::address( "127.0.0.1" , 7543 ));
	tdk::io::ip::address::resolve( "google.co.kr" ,  443  , addrs , AF_INET );
	c.connect( addrs , &c );
	l.run();
#endif
	
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

