#include "stdafx.h"
#include <tdk/error/error_category_decorator.hpp>
namespace tdk {
	
error_category_decorator::error_category_decorator( 
	const std::string& deco , const std::error_category& base )
	: _base_category(base)
	, _deco( deco ) {
}

error_category_decorator::~error_category_decorator(void) _NOEXCEPT {

}

const char *error_category_decorator::name(void) const _NOEXCEPT {
	static std::string name( _deco + " " + _base_category.name() );
	return name.c_str();
}

std::string error_category_decorator::message(int _Errval) const {
	std::string msg = _deco + " " + _base_category.message(_Errval);
}

}
