#include "stdafx.h"
#include "marker.hpp"

namespace tdk { namespace buffer {

const uint32_t marker::INVALID_MARK = 0xffffffff;

marker::marker( uint32_t read_mark , uint32_t write_mark  ) 
	: _read_mark( read_mark ) , _write_mark( write_mark ) 
{
}

marker::marker( const marker& rhs ) 
	: _read_mark( rhs._read_mark ) , _write_mark( rhs._write_mark ) {
}

marker::~marker( void ) {

}

uint32_t marker::read_mark( void ) const {
	return _read_mark;
}

uint32_t marker::write_mark( void ) const {
	return _write_mark;
}

marker& marker::operator=( const marker& rhs ) {
	_read_mark = rhs._read_mark;
	_write_mark = rhs._write_mark;	
	return *this;
}

}}