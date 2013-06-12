#include "stdafx.h"
#include <tdk/buffer/allocator.hpp>

namespace tdk {
namespace buffer {

allocator::allocator( void ){
	this->alloc		= std::malloc;
	this->free		= std::free;
	this->realloc	= std::realloc;
}

allocator::~allocator( void ){
}

}}
