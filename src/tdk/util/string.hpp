#ifndef __tdk_string_h__
#define __tdk_string_h__

#include <string>

namespace tdk {
namespace string {

std::string  wcs_to_mbs( const std::wstring& in );
std::wstring mbs_to_wcs( const std::string&  in );

std::string  mbs_to_utf_8( const std::string& in );
std::string  wcs_to_utf_8( const std::wstring& in );

std::wstring utf_8_to_wcs( const std::string& in );
std::string  utf_8_to_mbs( const std::string& in );


std::string& append_format( std::string& msg , const char* format , ... );
std::wstring& append_format( std::wstring& msg , const wchar_t* format , ... );	

}}

#endif