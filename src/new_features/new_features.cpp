// new_features.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <thread>
#include <mutex>
void run( void ) {
	printf_s( "Run!!\n" );
}

int _tmain(int argc, _TCHAR* argv[])
{

	std::mutex mtx;
	std::recursive_mutex mtx2;
	std::lock_guard<std::mutex> guard( mtx );

	std::thread t(run);

	t.join();
	return 0;
}

