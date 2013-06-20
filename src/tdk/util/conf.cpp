#include "StdAfx.h"
#include "conf.hpp"
#include <string>
#include <fstream>
#include <tdk/util/string.hpp>

namespace tdk { 
namespace util {
conf::conf( const tdk::tstring& file ) 
	: _filename( file ) {

}

conf::~conf(void) {
}

bool conf::load( void ) {
	std::ifstream file( tdk::string::wcs_to_mbs( _filename ) , std::ios::in );
	if ( !file ) {
		tdk::set_last_error( tdk::platform::error( ERROR_FILE_INVALID ));
		return false;
	}
	char read_buffer[4096];
	while ( file.getline( read_buffer , 4096 ) ) {
		std::string line(read_buffer);
		if ( line.length() == 0 || line[0] == '#' || line.length() < 3 ) {
			continue;
		}		
		if ( line.length() > 3 ) {
			std::vector< std::string > out;
			
			tdk::string::split( line , std::string("=") , std::back_inserter(out) );
			if ( out.size() == 2 ) {
				_value.insert( std::map< tdk::tstring , tdk::tstring >::value_type( 
					tdk::string::mbs_to_wcs( out[0] ) 
					, tdk::string::mbs_to_wcs( out[1] )));
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
