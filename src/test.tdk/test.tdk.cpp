// test.tdk.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <tdk/error/platform_error.hpp>
#include <tdk/util/string.hpp>
#include <tdk/log/logger.hpp>
#include <tdk/log/writer/console_writer.hpp>
#include <tdk/network/address.hpp>



#if defined ( _DEBUG )
	#pragma comment( lib , "gtest_x86_debug_mtd")
#else
	#pragma comment( lib , "gtest_x86_release_mt")
#endif

#pragma comment( lib , "tdk.lib")
#pragma comment( lib , "tdk.task.lib")
#pragma comment( lib , "ws2_32")
#pragma comment( lib , "mswsock")

#include <iostream>
#include <string>
#include <atomic>

#include <gtest/gtest.h>

int _tmain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest( &argc , argv );

	tdk::init();

	tdk::log::logger log("test.logger");
	log.add_writer( tdk::log::console_writer::instance() );
	LOG_D( log , _T("한글 %d") , 1 );	
	LOG_D( log , "%s" , tdk::platform_error(0).message().c_str());
	LOG_D( log , _T("%s") , tdk::string::mbs_to_wcs(tdk::platform_error(0).message()).c_str());
	tdk::network::address addr("google.co.kr" , 80 );
	LOG_D( log , "%s"  , addr.ip_address().c_str());
	
	return RUN_ALL_TESTS();
	/*
	// default C
	std::string old_locale = setlocale( LC_ALL , nullptr );
	// 한글에선 cp949
	old_locale = setlocale( LC_ALL , "" );
	//std::cout << tdk::string::utf_8_to_mbs( tdk::platform_error(0).message() )<< std::endl;
	*/
	//std::wcout << tdk::string::utf_8_to_wcs( tdk::platform_error( 0 ).message() ) << std::endl;
	
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

