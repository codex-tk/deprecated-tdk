#include "stdafx.h"
#include <tdk/memory/object_allocator.hpp>
#include <tdk/memory/alloc.hpp>

namespace tdk { namespace memory {

void* object_allocator::operator new ( std::size_t sz ) {
	return tdk::memory::alloc( sz );
}

void  object_allocator::operator delete ( void* ptr ) {
    TDK_ASSERT( ptr != nullptr );
	return tdk::memory::free( ptr );
}

// replacement new
void* object_allocator::operator new ( std::size_t sz , void* p) {
    TDK_ASSERT( p != nullptr );
    return p;
}
// replacement delete
void  object_allocator::operator delete( void* , void* ) {
}

}}
