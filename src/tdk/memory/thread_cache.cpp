#include "stdafx.h"
#include <tdk/memory/thread_cache.hpp>

namespace tdk { namespace memory {

thread_cache::thread_cache( void ) {
	for ( int i = 0; i < ALLOC_SIZE_TABLE_COUNT ; ++i ) {
		_allocator[i].init( ALLOC_SIZE_TABLE[i] );
	}
}

thread_cache::~thread_cache( void ) {
	
}

void* thread_cache::alloc( std::size_t sz ) {
	for ( int i = 0; i < ALLOC_SIZE_TABLE_COUNT ; ++i ) {
		if ( sz <= ALLOC_SIZE_TABLE[i] ) {
#if defined ( ENABLE_MEMORY_STATISTICS )
			return _allocator[i].alloc( statistics::tc_statistics[i] );
#else 
			return _allocator[i].alloc();
#endif	
		}
	}
	return lib_allocator::alloc( sz );
}

void thread_cache::free( void* p ) {
	if ( p == nullptr ) {
		TDK_ASSERT( !"[thread_cache::free()] param p is nullptr" );
		return;
	}
	block_info* bi = static_cast<block_info*>(p) - 1;
	if ( !bi->is_valid()) {
		TDK_ASSERT( !"[thread_cache::free()] block_info not valid");
		return;
	}
	if ( bi->is_variable() ) {
		lib_allocator::free( p );
		return;
	}
	std::size_t idx = bi->size();
	if ( idx < 0 || idx >= ALLOC_SIZE_TABLE_COUNT ) {
		TDK_ASSERT( !"[thread_cache::free()] block_info index error" );
		return;
	}
#if defined ( ENABLE_MEMORY_STATISTICS )
	_allocator[idx].free( p , statistics::tc_statistics[idx] );
#else 
	_allocator[idx].free( p );
#endif
}
	
void* thread_cache::realloc( void* p , std::size_t sz ) {
	if ( p == nullptr )
        return this->alloc( sz );

    std::size_t buf_size = this->size( p );
		
    if ( sz <= buf_size )
        return p;
		
    void* ptr = this->alloc( sz );		

	if ( ptr != nullptr )
		memcpy( ptr , p , buf_size );

    this->free( p );
	return ptr;	
}

std::size_t thread_cache::size( void* p ) {
	if ( p == nullptr ) {
		TDK_ASSERT( !"[thread_cache::size()] param p is nullptr" );
		return 0;
	}
	block_info* bi = static_cast<block_info*>(p) - 1;
	if ( !bi->is_valid()) {
		TDK_ASSERT( !"[thread_cache::size()] block_info not valid");
		return 0;
	}
	if ( bi->is_variable() ) {
		return bi->size();;
	}
	std::size_t idx = bi->size();
	if ( idx < 0 || idx >= ALLOC_SIZE_TABLE_COUNT ) {
		TDK_ASSERT( !"[allocator::size()] block_info index error" );
		return 0;
	}
	return ALLOC_SIZE_TABLE[idx];
}

}}
