#include "stdafx.h"
#include <tdk/buffer/allocator.hpp>
#include <tdk/alloc.hpp>
namespace tdk {
namespace buffer {

allocator::allocator( void ){
	this->alloc		= tdk::alloc;
	this->free		= tdk::free;
	this->realloc	= tdk::realloc;
}

allocator::~allocator( void ){
}

}}
