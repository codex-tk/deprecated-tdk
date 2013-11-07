#include "stdafx.h"
#include <tdk/util/win32/env.hpp>
#include <tdk/util/string.hpp>

namespace tdk {
namespace util {
	
namespace {

int __stdcall ServicePack( OSVERSIONINFOEX& info ) {
    int nServicePack = 0;
    for ( int i = 0 ;
		( info.szCSDVersion[i] != _T('\0') ) && ( i < (sizeof(info.szCSDVersion) / sizeof(TCHAR) ) ) ; 
		i++ ){
	    if ( _istdigit( info.szCSDVersion[i] ) ) {
		    nServicePack = _ttoi( &info.szCSDVersion[i] );
		    break;
	    }
    }
    return nServicePack;
}

}

int __stdcall env::page_size( void ) {
	return 4096;
}

int __stdcall env::number_of_process( void ) {
    SYSTEM_INFO systemInfo;
    GetSystemInfo( &systemInfo );
    return systemInfo.dwNumberOfProcessors;
}

env::version __stdcall env::os_version( void ) {
    OSVERSIONINFOEX info;
    memset( &info , 0x00 , sizeof( info ));
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO*)&info);

    SYSTEM_INFO sysInfo;
    GetSystemInfo( &sysInfo );

    switch ( info.dwPlatformId ) {
    case VER_PLATFORM_WIN32s:
		return version::windows_31;
    case VER_PLATFORM_WIN32_WINDOWS:
        {
            switch ( info.dwMinorVersion ) {
            case 0:
                if ( info.szCSDVersion[1] == 'B' ) 
                    return version::windows_95osr2;
                else 
                    return version::windows_95;
            case 10:
                if ( info.szCSDVersion[1] == 'A' ) 
                    return version::windows_98se;
                else 
                    return version::windows_98;
            case 90:
                if ( info.dwBuildNumber == 73010104 ) 
                    return version::windows_me;
                else 
                    return version::windows_9x;
            }
        }
    case VER_PLATFORM_WIN32_NT:
        {
            switch ( info.dwMajorVersion ) {
            case 3: return version::windows_nt3;
            case 4:  
                {
                    return version::windows_nt4;   
                }
            case 5: 
                {
                    switch ( info.dwMinorVersion ) {
                    case 0 : return version::windows_2000; 
                    case 1 : return version::windows_xp; 
                    case 2 :  
                        {
                            if (( info.wProductType == VER_NT_WORKSTATION) && 
				                ( sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) ) {
				                return version::windows_xpprofessionalx64edition;
			                }
                #if ( _MSC_VER >= 1600 )
			                if ( info.wSuiteMask & VER_SUITE_WH_SERVER ) {
				                return version::windows_homeserver;
			                }
                #endif
			                int metrics = GetSystemMetrics(SM_SERVERR2);
			                if ( metrics == 0 ) {
				                return version::windows_server2003;
			                } else {
				                return version::windows_server2003r2;
			                }
                        }
                    }
                }
            case 6:
                {
                    switch ( info.dwMinorVersion ) {
		            case 0:
			            if ( info.wProductType == VER_NT_WORKSTATION ) {
				            return version::windows_vista;
			            } else {
				            return version::windows_server2008;
			            }
		            case 1:
			            if ( info.wProductType == VER_NT_WORKSTATION ) {
				            return version::windows_7;
			            } else {
				            return version::windows_server2008r2;
			            }
		            }
                }
            }
        }
    }

	return version::unknown;
}

tdk::tstring         __stdcall env::process_name( void ) {
	TCHAR filePath[MAX_PATH];
	GetModuleFileName( nullptr , filePath , MAX_PATH );
	TCHAR *fileName = _tcschr( filePath , '\\' );
	if ( fileName ) {
		return tdk::tstring( fileName );
	}
	return tdk::tstring( _T("") );
}

tdk::tstring         __stdcall env::process_directory( void ){
	TCHAR filePath[MAX_PATH];
	GetModuleFileName( nullptr , filePath , MAX_PATH );
	TCHAR *fileName = _tcsrchr( filePath , '\\' );
	if ( fileName ) {
		*(++fileName) = '\0';	
		return tdk::tstring( fileName );
	}
	return tdk::tstring( _T("") );
	/*

	wchar_t filePath[MAX_PATH];
	GetModuleFileName( nullptr , filePath , MAX_PATH );
	wchar_t *fileName = wcsrchr( filePath , '\\' );
	if ( fileName ) {
		*(++fileName) = '\0';
		return caffe::tools::string_op::WtoA( std::wstring( filePath ) );
	}
	return std::string( "" );*/
}

tdk::tstring         __stdcall env::current_directory( void ){
	TCHAR filePath[MAX_PATH];
	GetCurrentDirectory( MAX_PATH , filePath );
	return tdk::tstring( filePath );
}


tdk::tstring __stdcall env::process_path( void ) {
	TCHAR path[MAX_PATH];
	GetModuleFileName( nullptr , path , MAX_PATH );
	return tdk::tstring( path );
}

tdk::tstring		__stdcall env::os_version_string( void ) {
	switch ( os_version() ) {
		case version::unknown                          	 : return _T("unknown");
		case version::windows_31                       	 : return _T("windows_31");
		case version::windows_9x                       	 : return _T("windows_9x");
		case version::windows_95                       	 : return _T("windows_95");
		case version::windows_95osr2                   	 : return _T("windows_95osr2");
		case version::windows_98                       	 : return _T("windows_98");
		case version::windows_98se                     	 : return _T("windows_98se");
		case version::windows_me                       	 : return _T("windows_me");
		case version::windows_nt                       	 : return _T("windows_nt");
		case version::windows_nt3                      	 : return _T("windows_nt3");
		case version::windows_nt4                      	 : return _T("windows_nt4");
		case version::windows_2000                     	 : return _T("windows_2000");
		case version::windows_xphome                   	 : return _T("windows_xphome");
		case version::windows_xp                       	 : return _T("windows_xp");
		case version::windows_xpprofessionalx64edition 	 : return _T("windows_xpprofessionalx64edition");
		case version::windows_server2003               	 : return _T("windows_server2003");
		case version::windows_homeserver               	 : return _T("windows_homeserver");
		case version::windows_server2003r2             	 : return _T("windows_server2003r2");
		case version::windows_vista                    	 : return _T("windows_vista");
		case version::windows_server2008               	 : return _T("windows_server2008");
		case version::windows_server2008r2             	 : return _T("windows_server2008r2");
		case version::windows_7                        	 : return _T("windows_7");
	}
	return _T("unknown");
}



}}
