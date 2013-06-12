#ifndef __tdk_buffer_allocator_h__
#define __tdk_buffer_allocator_h__

namespace tdk {
namespace buffer {

class allocator{
public:
	allocator( void );
	~allocator( void );
	// align required
    void* (*alloc)( std::size_t sz ) ;
    void (*free)( void* p ) ;
    void* (*realloc)( void* p , std::size_t sz );
};

}}

#endif