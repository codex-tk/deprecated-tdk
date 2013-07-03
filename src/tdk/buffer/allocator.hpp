#ifndef __tdk_buffer_allocator_h__
#define __tdk_buffer_allocator_h__
#include <tdk/alloc.hpp>
namespace tdk {
namespace buffer {

class allocator{
public:
	allocator( void );
	~allocator( void );
	// align required
	tdk::alloc_handler alloc;
	tdk::realloc_handler realloc;
	tdk::free_handler free;
};

}}

#endif