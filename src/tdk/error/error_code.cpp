#include "stdafx.h"
#include <tdk/error/error_code.hpp>
#include <tdk/error/error_category.hpp>
#include <tdk/error/error_platform.hpp>
namespace tdk { 

error_code::error_code( void )
	: _value(0) , _category( &platform::category() ){
}

error_code::error_code(int ec , const error_category& category)
	: _value( ec ) , _category( &category ){
}

error_code::error_code( const error_code& rhs )
	: _value( rhs._value ) , _category( rhs._category ){
}


error_code error_code::operator=( const error_code& rhs ) {
	_value = rhs._value;
	_category = rhs._category;
	return *this;
}


error_code::~error_code(void)
{
}


tdk::tstring error_code::message( void ) const {
	return _category->message( _value );
}

int error_code::value( void ) const {
	return _value;
}


const error_category& error_code::category( void ) const {
	return *_category;
}


error_code::operator unspecfied_bool_type() const  {
	return _value == 0 ? nullptr : &error_code::value;
}

}