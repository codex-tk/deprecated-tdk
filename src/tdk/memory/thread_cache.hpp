#ifndef __tdk_memory_thread_cache_h__
#define __tdk_memory_thread_cache_h__

#include <tdk/memory/allocator.hpp>
#include <tdk/memory/alloc.hpp>
#include <tdk/threading/spin_lock.hpp>

namespace tdk { namespace memory {

class thread_cache {
public:
	thread_cache( void );
	~thread_cache( void );

	void* alloc( std::size_t sz );
	void free( void* p );
	void* realloc( void* p , std::size_t sz );

	std::size_t size( void* p );
private:
	tdk::memory::fixed_size_allocator<
		tdk::threading::no_lock , lib_allocator 
	> _allocator[ALLOC_SIZE_TABLE_COUNT];
};


}}


#endif