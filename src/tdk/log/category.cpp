#include "stdafx.h"
#include <tdk/log/category.hpp>

namespace tdk {
namespace log {

category::category( const std::string& name )
	: _name( name ) 
	, _hash(0)
{
	for ( std::size_t i = 0 ; i < name.length() ; ++i ) {
		_hash += name[i];
	}
}

category::~category( void ) {

}

const std::string& category::name( void ) const {
	return _name;
}

const uint32_t category::hash( void ) const {
	return _hash;
}

}}