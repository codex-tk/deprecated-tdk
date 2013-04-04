#include "stdafx.h"
#include <tdk/util/string.hpp>
#include <vector>

namespace tdk{
namespace string {

std::string  wcs_to_mbs( const std::wstring& in ) {
	std::string out;
    std::size_t length = in.length();
    if ( length != 0 ) {
        int numOfChar = WideCharToMultiByte( CP_ACP , 0 , in.c_str() , -1 , nullptr , 0 , nullptr , nullptr );
        if ( numOfChar != 0 ) {
            std::vector< char > result(numOfChar);
            WideCharToMultiByte( CP_ACP , 0 , in.c_str() , -1 , &result[0] , numOfChar , nullptr , nullptr );
            out = &result[0];
        }
    }
    return out;
}

std::wstring mbs_to_wcs( const std::string&  in ) {
	std::wstring out;
    std::size_t length = in.length();
    if ( length != 0 ) {
        // null term 포함 필요한 문자의 갯수를 리턴
        int numOfChar = MultiByteToWideChar( CP_ACP , 0 , in.c_str() , -1 , nullptr , 0 );
        if ( numOfChar != 0 ) {
            std::vector< wchar_t > result(numOfChar);
            MultiByteToWideChar( CP_ACP , 0 , in.c_str() , -1 , &result[0] , numOfChar );
            out = &result[0];
        }
    }
    return out;
}

std::string  wcs_to_utf_8( const std::wstring& in ) {
	std::string out;
	std::size_t length = in.length();
	if ( length != 0 ) {
		int numOfChar =WideCharToMultiByte( CP_UTF8 , 0 , in.c_str()  , -1  , nullptr , 0  , nullptr , nullptr );
		if ( numOfChar != 0 ) {
			std::vector< char > result(numOfChar);
			WideCharToMultiByte( CP_UTF8  , 0 , in.c_str() , -1  ,  &result[0] , numOfChar , nullptr , nullptr );
			out = &result[0];
		}
	}
	return out;
}

std::wstring utf_8_to_wcs( const std::string& in ){
	std::wstring out;
    std::size_t length = in.length();
    if ( length != 0 ) {
		int numOfChar = MultiByteToWideChar( CP_UTF8 , 0 , in.c_str() , -1 , nullptr , 0 );
        if ( numOfChar != 0 ) {
            std::vector< wchar_t > result(numOfChar);
            MultiByteToWideChar( CP_UTF8 , 0 , in.c_str() , -1 , &result[0] , numOfChar );
            out = &result[0];
        }
    }
    return out;
}



}}