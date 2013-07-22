#include "stdafx.h"
#include <tdk/error/error_category_platform.hpp>
#include <tdk/util/string.hpp>

#include <sstream>

namespace tdk {
namespace platform {

error_category_platform::error_category_platform(void) {

}

error_category_platform::~error_category_platform(void) _NOEXCEPT {

}

const char *error_category_platform::name() const _NOEXCEPT  {
#if defined( _WIN32 )
	return "win32_platform_error";
#elif defined( linux ) || defined ( __linux )
	return "linux_platform_error";
#elif defined ( __MACOSX__ ) || defined ( __APPLE__ ) 
	return "maxosx_platform_error";
#else

#endif
}

std::string error_category_platform::message(int _Errval) const  {
#if defined ( _WIN32 ) 
	const size_t buffer_size = 4096;
    DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM;
    LPCVOID lpSource = NULL;
	char buffer[buffer_size] = {0,};
	unsigned long result;
    result = ::FormatMessageA(
        dwFlags,
        lpSource,
        _Errval,
        0,
        buffer,
        buffer_size,
        NULL);

    if (result == 0) {
        std::ostringstream os;
        os << "Unable to get an error message for error code: " << _Errval << ".";
        return os.str();
    }
	return std::string( buffer );

#elif defined( linux ) || defined ( __linux )
    const size_t buffer_size = 4096;
	char buffer[buffer_size] = {0,};
	char* msg = strerror_r( _Errval , buffer , buffer_size);
	return std::string( msg );
#elif defined ( __MACOSX__ ) || defined ( __APPLE__ ) 
#else
#endif
}

/*
error_category_platform::error_category_platform(void) {

}

error_category_platform::~error_category_platform(void) {

}

const TCHAR* error_category_platform::name() const {
	return _T("error_category_platform");
}

tdk::tstring error_category_platform::message(int ec) const  {
	const size_t buffer_size = 4096;
    DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM;
    LPCVOID lpSource = NULL;*/
	/*
#if !defined(__cplusplus_winrt)
    if (ec >= 12000) {
        dwFlags = FORMAT_MESSAGE_FROM_HMODULE;
        lpSource = GetModuleHandleA("winhttp.dll"); // this handle DOES NOT need to be freed
    }
#endif*/
	/*
	TCHAR buffer[buffer_size] = {0,};
	unsigned long result;
    result = ::FormatMessage(
        dwFlags,
        lpSource,
        ec,
        0,
        buffer,
        buffer_size,
        NULL);

    if (result == 0) {
		std::wstringstream os;
        //std::ostringstream os;
        os << L"Unable to get an error message for error code: " << ec << L".";
        return os.str();
    }
	return tdk::tstring(buffer);*/
	/*
	char buffer[buffer_size] = {0,};
	unsigned long result;
    result = ::FormatMessageA(
        dwFlags,
        lpSource,
        ec,
        0,
        buffer,
        buffer_size,
        NULL);

    if (result == 0) {
        std::ostringstream os;
        os << "Unable to get an error message for error code: " << ec << ".";
        return os.str();
    }
	return std::string( buffer );*/
//}

}}
