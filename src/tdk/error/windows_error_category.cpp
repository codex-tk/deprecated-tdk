#include "stdafx.h"

#include <tdk/error/windows_error_category.hpp>
#include <tdk/util/string.hpp>

#include <sstream>

namespace tdk {

windows_error_category::windows_error_category(void) {

}

windows_error_category::~windows_error_category(void) {

}

const char* windows_error_category::name() const {
	return "windows_error_category";
}

std::string windows_error_category::message(int ec) const  {
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

	wchar_t buffer[buffer_size] = {0,};
	unsigned long result;
    result = ::FormatMessageW(
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
	return tdk::string::wcs_to_utf_8( buffer );
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

}