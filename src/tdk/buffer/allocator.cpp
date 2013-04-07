#include "stdafx.h"
#include <tdk/buffer/allocator.hpp>

namespace tdk {
namespace buffer {

allocator::allocator( void ){
}

allocator::~allocator( void ){
}

void* allocator::alloc( std::size_t sz ) {
	return std::malloc( sz );
}

void allocator::free( void* p ){
	return std::free( p );
}

void* allocator::realloc( void* p , std::size_t sz ){
	return std::realloc( p , sz );
}

}}
