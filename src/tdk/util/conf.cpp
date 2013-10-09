#include "stdafx.h"
#include <tdk/util/conf.hpp>
#include <string>
#include <iostream>
#include <tdk/util/string.hpp>
#include <tdk/error/error_platform.hpp>
#include <stdlib.h>
#include <vector>

namespace tdk { 
namespace util {
conf::conf( const tdk::tstring& file ) 
	: _filename( file ) {

}

conf::~conf(void) {
}

bool conf::load( void ) {
/*
	std::ifstream fs(
#if defned( __WIN32 )
			tdk::string::wcs_to_mbs( _filename )
#else
		_filename
#endif
	, std::ios::in );
	if ( !fs ) {
#if defned( __WIN32 )
		tdk::set_last_error( tdk::platform::error( ERROR_FILE_INVALID ));
#endif
		return false;
	}*/

	FILE* fp = fopen( _filename.c_str() , "r");

	char * read_buffer = nullptr;
	int read_size = 0;
	size_t len = 0;
	while ( ( read_size = getline( &read_buffer , &len , fp)) != 0 ) {
		std::string line(read_buffer);
		if ( line.length() == 0 || line[0] == '#' || line.length() < 3 ) {
			continue;
		}		
		if ( line.length() > 3 ) {
			std::vector< std::string > out;
			tdk::string::split( line , std::string("=") , std::back_inserter(out) );
			if ( out.size() == 2 ) {
				_value.insert( std::map< tdk::tstring , tdk::tstring >::value_type(
						out[0] , out[1] ));
/*
					tdk::string::mbs_to_wcs( out[0] ) 
					, tdk::string::mbs_to_wcs( out[1] )));
					*/
			}
		}
	}	
	return true;
}

tdk::tstring conf::operator[]( const tdk::tstring& str ) {
	std::map< tdk::tstring , tdk::tstring >::iterator it = _value.find( str  );
	if ( it == _value.end() ) {
		return nullptr;
	}
	return it->second;
}
}}
