#include "StdAfx.h"
#include <tdk/util/win32/ini_file.hpp>
namespace tdk {
namespace util {

ini_file::ini_file( const tdk::tstring& fileName , const tdk::tstring& defString , const int defInt )
	: _file_name( fileName ) , _def_string( defString ) , _def_int( defInt ) {
}

ini_file::~ini_file(void) {

}

ini_section ini_file::operator[]( const tdk::tstring& section ){
	return ini_section( *this , section );
}

const tdk::tstring& ini_file::name( void ) const {
    return _file_name;
}

bool ini_file::is_exist( void ) const {
    DWORD       fileattr;
	fileattr = GetFileAttributes( _file_name.c_str() );
    if (INVALID_FILE_ATTRIBUTES == fileattr) {
        return false;
    }
    return true;
}

}}