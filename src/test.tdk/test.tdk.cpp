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
	
}

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

