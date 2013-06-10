#include "stdafx.h"

#include <tdk/error/error_category_platform.hpp>
#include <tdk/util/string.hpp>

#include <sstream>

namespace tdk {
namespace platform {
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
    LPCVOID lpSource = NULL;
	/*
#if !defined(__cplusplus_winrt)
    if (ec >= 12000) {
        dwFlags = FORMAT_MESSAGE_FROM_HMODULE;
        lpSource = GetModuleHandleA("winhttp.dll"); // this handle DOES NOT need to be freed
    }
#endif*/

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
	return tdk::tstring(buffer);
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
}

}}