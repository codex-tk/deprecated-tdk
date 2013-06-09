#ifndef __tdk_buffer_allocator_h__
#define __tdk_buffer_allocator_h__

namespace tdk {
namespace buffer {

class allocator{
public:
	allocator( void );
	virtual ~allocator( void );
	// align required
	virtual void* alloc( std::size_t sz ) ;
	virtual void free( void* p ) ;
	virtual void* realloc( void* p , std::size_t sz ) ;
};

}}

#endif