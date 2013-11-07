// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TDKCORE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TDKCORE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifndef __tdk_h__
#define __tdk_h__

// 윈도우와 그 이외의 플랫폼에 대한 설정
#if defined ( _WIN32 )

#pragma comment( lib ,"ws2_32" )
#pragma comment( lib ,"mswsock" )
#pragma comment( lib ,"tdk" )

template < typename T >
struct dword_align {
	typedef __declspec(align( 4 )) T type;
};
template < typename T >
struct qword_align {
	typedef __declspec(align( 8 )) T type;
};

#if ( _MSC_VER < 1600 ) 
	#define nullptr NULL
#endif

#ifndef COMPLETION_PORT_SEH
#define COMPLETION_PORT_SEH
#endif

#ifndef OPERATION_SEH
#define OPERATION_SEH
#endif

#else

#ifndef __stdcall
#define __stdcall
#endif

#ifndef TCHAR
#define TCHAR char
#endif

#ifndef _T
#define _T( x ) x
#endif

#include <string>
#include <cstdint>
#include <sys/time.h>
#include <system_error>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

#ifndef _NOEXCEPT
#define _NOEXCEPT noexcept 
#endif
#endif

/*
Darwin	 DARWIN						Darwin is not necessarily OS X, see below
FreeBSD	 __FreeBSD__				Consider using the BSD macro
Linux	 linux or __linux			I also found LINUX
MacOS X	 __MACOSX__ or __APPLE__	
NetBSD	 __NetBSD__					Consider using the BSD macro
OpenBSD	 __OpenBSD__				Consider using the BSD macro
Solaris	 sun or __sun				SunOS versions < 5 will not have __SVR4 or __svr4__ defined
Windows	 _WIN32 or __WIN32__	
*/


// 개별 플랫폼 상세 설정
#if defined ( _WIN32 ) 
#pragma warning( disable : 4819 )
#elif defined( linux ) || defined ( __linux )

#elif defined ( __MACOSX__ ) || defined ( __APPLE__ ) 

#else

#endif

#ifndef assert_msg
#define assert_msg( expr , msg ) assert( expr && msg )
#endif

#ifndef TDK_ASSERT
#define TDK_ASSERT( expr ) do{ if ( tdk::option::enable_assert ) assert( expr ); }while(0)
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


#ifndef TDK_DELETE
#define TDK_DELETE( obj ) \
	if ( obj ) {\
		delete obj;\
		obj = nullptr;\
	}

#endif

namespace tdk {
namespace option {  extern bool enable_assert;  }

bool init( void );
void cleanup( void );

typedef std::basic_string<TCHAR
	, std::char_traits<TCHAR>
	, std::allocator<TCHAR> > tstring;

}

#include <tdk/tmp/compile_time_error.hpp>

#define STATIC_CHECK( expr , msg ) { tdk::tmp::compile_time_error< (expr) != 0 > ERROR_##msg;(void)ERROR_##msg;}

#include <tdk/util/initializer.hpp>

#ifndef STATIC_INITIALIZE
#define STATIC_INITIALIZE( name , func ) \
namespace {\
	tdk::util::initializer _initializer_##name( func );\
}
#endif

//STATIC_INITIALIZE( tdk_auto_init , tdk::init );

#endif



