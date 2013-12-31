#ifndef __tdk_ssl_h__
#define __tdk_ssl_h__

#include <tdk.ssl/open_ssl.hpp>
#include <tdk.ssl/context.hpp>
#include <tdk.ssl/filter.hpp>

#if defined (_WIN32)
	#if defined (_WIN64) 
		#pragma comment( lib , "x64/out32dll/libeay32")
		#pragma comment( lib , "x64/out32dll/ssleay32")
	#else
		#pragma comment( lib , "x86/out32dll/libeay32")
		#pragma comment( lib , "x86/out32dll/ssleay32")
	#endif
#endif

#endif