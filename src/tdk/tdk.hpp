// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TDKCORE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TDKCORE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifndef __tck_h__
#define __tck_h__

#pragma comment( lib ,"ws2_32.lib" )
#pragma comment( lib ,"mswsock.lib" )
#pragma comment( lib ,"tdk.lib" )
/*
#if defined( _M_X64 ) 
	#if defined( _DEBUG )
		#pragma comment( lib ,"tdk.Debug.x64.lib" )
	#else
		#pragma comment( lib ,"tdk.Release.x64.lib" )
	#endif
#else
	#if defined( _DEBUG )
		#pragma comment( lib ,"tdk.Debug.Win32.lib" )
	#else
		#pragma comment( lib ,"tdk.Release.Win32.lib" )
	#endif
#endif
*/
/*
#include <WinSock2.h>
#include <MSWSOCK.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <process.h>
#include <stdint.h>
*/

namespace tdk {
namespace option {

extern bool enable_assert;

}}

#ifndef assert_msg
#define assert_msg( expr , msg ) assert( expr && msg )
#endif

#ifndef TDK_ASSERT
#define TDK_ASSERT( expr ) do{ if ( tdk::option::enable_assert ) assert( expr ); }while(0)
#endif

namespace tdk {

class initializer {
public:
	template < typename function >
	initializer( function func ) {
		func();
	} 
};

#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

#if defined( _WIN32 )
#if ( _MSC_VER < 1600 ) 
	#define nullptr NULL
#endif

template < typename T >
struct dword_align {
#if defined ( _MSC_VER )
	typedef __declspec(align( 4 )) T type;
#endif
};

template < typename T >
struct qword_align {
#if defined ( _MSC_VER )
	typedef __declspec(align( 8 )) T type;
#else
#endif
};

#endif
}

#ifndef STATIC_INITIALIZE
#define STATIC_INITIALIZE( name , func ) \
namespace {\
	static tdk::initializer _initializer##name( func );\
}
#endif


#ifndef DISALLOW_COPY
#define DISALLOW_COPY( className ) \
	className( const className& rhs );\
	className& operator=( const className& rhs);
#endif

#ifndef DISALLOW_CONSTRUCTOR
#define DISALLOW_CONSTRUCTOR( className ) \
	className( void );\
	~className( void );
#endif

#ifndef DISALLOW_CONSTRUCTOR_AND_COPY
#define DISALLOW_CONSTRUCTOR_AND_COPY( className )\
	DISALLOW_CONSTRUCTOR( className );\
	DISALLOW_COPY( className );
#endif

template < bool > class compile_time_error;
template <> class compile_time_error< true >{};

#define STATIC_CHECK( expr , msg ) { compile_time_error< (expr) != 0 > ERROR_##msg;(void)ERROR_##msg;}

#ifndef COMPLETION_PORT_SEH
#define COMPLETION_PORT_SEH
#endif

#ifndef COMPLETION_PORT_USE_GQCS_EX
#define COMPLETION_PORT_USE_GQCS_EX
#endif

#ifndef OPERATION_SEH
#define OPERATION_SEH
#endif

#ifndef TDK_DELETE
#define TDK_DELETE( obj ) \
	if ( obj ) {\
		delete obj;\
		obj = nullptr;\
	}

#endif

#endif



