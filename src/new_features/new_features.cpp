// new_features.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <thread>
#include <mutex>
#include <tdk/tdk.hpp>

#include <tdk/error/error_platform.hpp>
#include <tdk/error/error_tdk.hpp>
#include <tdk/time/date_time.hpp>

void run( void ) {
	printf_s( "Run!!\n" );
}

#pragma comment( lib , "tdk.lib")
#pragma comment( lib , "ws2_32")
#pragma comment( lib , "mswsock")

int _tmain(int argc, _TCHAR* argv[])
{
	tdk::init();

	tdk::date_time dt = tdk::date_time::local();

	std::string data = dt.to_string();


	std::mutex mtx;
	std::recursive_mutex mtx2;

	std::lock_guard<std::mutex> guard( mtx );

	const std::error_category& ca = std::system_category();

	const std::error_category& cate = tdk::platform::category();

	std::error_code ec( ERROR_IO_PENDING , ca);

	std::string msg = ec.message();

	msg = tdk::tdk_network_remote_closed.message();
	std::thread t(run);

	t.join();
	return 0;
}

