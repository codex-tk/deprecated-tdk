#include "stdafx.h"
#include <tdk/memory/alloc.hpp>
#include <tdk/memory/allocator.hpp>
#include <tdk/threading/thread_data.hpp>
#include <tdk/memory/memory_statistics.hpp>

namespace tdk { namespace memory { 
namespace {

tdk::memory::alloc_hook_fn _alloc_hook = &tc_allocator::alloc;
tdk::memory::realloc_hook_fn _realloc_hook = &tc_allocator::realloc;
tdk::memory::free_hook_fn _free_hook = &tc_allocator::free;

tdk::memory::allocator< 
	tdk::threading::spin_lock 
	, tdk::memory::malloc_allocator 
	, tdk::memory::fixed_size_allocator 
> lib_allocator_impl; 

}
void* malloc_allocator::alloc( std::size_t size ) {
	return std::malloc( size );
}
void* malloc_allocator::realloc( void* p , std::size_t size ) {
	return std::realloc( p , size );
}
void  malloc_allocator::free( void* p ) {
	return std::free( p  );
}

void* lib_allocator::alloc( std::size_t size ) {
	return lib_allocator_impl.alloc( size );
}
void* lib_allocator::realloc( void* p , std::size_t size ) {
	return lib_allocator_impl.realloc( p , size );
}
void  lib_allocator::free( void* p ) {
	return lib_allocator_impl.free( p );
}

void* tc_allocator::alloc( std::size_t size ) {
	void* ptr = nullptr;
	if ( tdk::threading::data* tls = tdk::threading::data::get_thread_data() ) {
		ptr = tls->cache.alloc( size );
	} else {
		//TDK_ASSERT( !"[tc_allocator::alloc()] tls nullptr" );
		ptr = lib_allocator::alloc( size );
	}
	return ptr;
}

void* tc_allocator::realloc( void* p , std::size_t size ) {
	void* ptr = nullptr;
	if ( tdk::threading::data* tls = tdk::threading::data::get_thread_data() ) {
		ptr = tls->cache.realloc( p , size );
	} else {
		//TDK_ASSERT( !"[tc_allocator::realloc()] tls nullptr" );
		ptr = lib_allocator::realloc( p , size );
	}
	return ptr;
}

void  tc_allocator::free( void* p ) {
	if ( tdk::threading::data* tls = tdk::threading::data::get_thread_data() ) {
		tls->cache.free( p );
	} else {
		//TDK_ASSERT( !"[tc_allocator::free()] tls nullptr" );
		lib_allocator::free( p );
	}
}

void* alloc( std::size_t size ) {
	return _alloc_hook(size);
}

void* realloc( void* p , std::size_t size ) {
	return _realloc_hook(p,size);
}

void  free( void* p ) {
	return _free_hook( p );
}

void  alloc_hook( alloc_hook_fn function ) {
	if ( function == nullptr ) {
		function = &tc_allocator::alloc;
	}
	_alloc_hook = function;
}

void  realloc_hook( realloc_hook_fn function ) {
	if ( function == nullptr ) {
		function = &tc_allocator::realloc;
	}
	_realloc_hook = function;
}

void  free_hook( free_hook_fn function ) {
	if ( function == nullptr ) {
		function = &tc_allocator::free;
	}
	_free_hook = function;
}

alloc_hook_fn alloc_hook( void ) {
	return _alloc_hook;
}

realloc_hook_fn realloc_hook( void ) {
	return _realloc_hook;
}

free_hook_fn free_hook( void ) {
	return _free_hook;
}



}}