// new_features.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <thread>
#include <mutex>
#include <tdk/tdk.hpp>
void run( void ) {
	printf_s( "Run!!\n" );
}

#pragma comment( lib , "tdk.lib")
#pragma comment( lib , "ws2_32")
#pragma comment( lib , "mswsock")

int _tmain(int argc, _TCHAR* argv[])
{
	tdk::init();
	std::mutex mtx;
	std::recursive_mutex mtx2;
	std::lock_guard<std::mutex> guard( mtx );

	std::thread t(run);

	t.join();
	return 0;
}

