#include "stdafx.h"
#include <tdk/log/category.hpp>

namespace tdk {
namespace log {

category::category( const tdk::tstring& name )
	: _hash(0)
	, _name( name )
{
	for ( std::size_t i = 0 ; i < name.length() ; ++i ) {
		_hash += name[i];
	}
}

category::~category( void ) {

}

const tdk::tstring& category::name( void ) const {
	return _name;
}

const uint32_t category::hash( void ) const {
	return _hash;
}

}}
