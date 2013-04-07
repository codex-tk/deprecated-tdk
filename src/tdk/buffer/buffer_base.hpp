#ifndef __tdk_buffer_buffer_base_h__
#define __tdk_buffer_buffer_base_h__

#include <tdk/tdk.hpp>
#include <tdk/buffer/allocator.hpp>
#include <atomic>

namespace tdk {
namespace buffer {
	
// reserve 는 상위 단에서 처리
class buffer_base {
public:
	buffer_base( std::size_t sz 
		, tdk::buffer::allocator* alloc = nullptr );

	buffer_base( void* base
		, std::size_t sz
		, tdk::buffer::allocator* alloc = nullptr );

	~buffer_base( void );

	buffer_base( const buffer_base& rhs );
	buffer_base& operator=( const buffer_base& rhs );

	int ref_count( void );

	uint8_t* data_ptr( void ) const;
	std::size_t size( void ) const;

	int retain( void );
	int release( void );

	void swap( buffer_base& rhs );

	void reserve( std::size_t sz );
private:
	std::atomic_int* _counter(void);
private:
	void* _base;
	std::size_t _size;
	tdk::buffer::allocator* _allocator;
};

}}

#endif