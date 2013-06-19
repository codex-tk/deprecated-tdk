#include "StdAfx.h"
#include "conf.hpp"
#include <string>
#include <fstream>
#include "string.hpp"

namespace tdk { 
namespace util {
	/*
conf::conf( const tdk::tstring& file ) 
	: _filename( file ) {

}

conf::~conf(void) {
}

bool conf::load( void ) {
	std::ifstream file( _filename.c_str() , std::ios::in );
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
			
			tdk::base::string::split( line , std::string("=") , std::back_inserter(out) );
			if ( out.size() == 2 ) {
				_value.insert( std::map< std::string , std::string >::value_type( out[0] , out[1] ));
			}
		}
	}	
	return true;
}

const char* conf::operator[]( const char* str ) {
	std::map< std::string , std::string >::iterator it = _value.find( str  );
	if ( it == _value.end() ) {
		return nullptr;
	}
	return it->second.c_str();
}
*/
}}
