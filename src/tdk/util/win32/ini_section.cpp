#include "StdAfx.h"
#include <tdk/util/win32/ini_section.hpp>
namespace tdk {
namespace util {

ini_section::ini_section( const ini_file& file , const tdk::tstring& section )
	: _ini_file( file ) , _section( section ) {

}

ini_section::~ini_section(void) {

}

ini_key ini_section::operator[]( const tdk::tstring& key ) {
    return ini_key( *this , key );
}

const ini_file& ini_section::file( void ) const {
    return _ini_file;
}

const tdk::tstring& ini_section::name( void ) const {
    return _section;
}

}}