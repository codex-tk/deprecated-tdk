#ifndef __tdk_string_h__
#define __tdk_string_h__

#include <string>

namespace tdk {
namespace string {

std::string  wcs_to_mbs( const std::wstring& in );
std::wstring mbs_to_wcs( const std::string&  in );

std::string  wcs_to_utf_8( const std::wstring& in );
std::wstring utf_8_to_wcs( const std::string& in );

}}

#endif