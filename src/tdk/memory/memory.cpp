#include "stdafx.h"
#include <tdk/memory/memory.hpp>

namespace tdk { namespace memory {
namespace {
#if !defined( _M_X64 ) 
const uint32_t TYPE_MASK		= 0x80000000; 
const uint32_t ID_MASK			= 0x60000000;
const uint32_t FREE_MARK_MASK	= 0x10000000;
const uint32_t DATA_MASK		= 0x0fffffff;

#else
const uint64_t TYPE_MASK		= 0x8000000000000000; 
const uint64_t ID_MASK			= 0x6000000000000000;
const uint64_t FREE_MARK_MASK	= 0x1000000000000000;
const uint64_t DATA_MASK		= 0x0fffffffffffffff;
#endif
}

block_info::block_info( bool variable , std::size_t sz ){
	data = ID_MASK | ( sz & DATA_MASK );
	if ( variable ) {
		data |= TYPE_MASK;
	}
}

block_info::~block_info( void ){

}	 

bool block_info::is_variable( void ){
	return ( data & TYPE_MASK ) != 0;
}

std::size_t block_info::size( void ){
	return data & DATA_MASK;
}

bool block_info::is_valid( void ){
	return ( data & ID_MASK ) == ID_MASK;
}

bool block_info::is_free_marked( void ) {
	return ( data & FREE_MARK_MASK ) == FREE_MARK_MASK;
}

void block_info::free_mark( bool mark ) {
	if ( mark ) {
		data |= FREE_MARK_MASK;
	} else {
		data &= ~FREE_MARK_MASK;
	}
}

}}