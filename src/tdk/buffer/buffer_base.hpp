#ifndef __tdk_buffer_buffer_base_h__
#define __tdk_buffer_buffer_base_h__

#include <tdk/tdk.hpp>
#include <tdk/buffer/allocator.hpp>
#include <tdk/threading/atomic/atomic.hpp>

namespace tdk {
namespace buffer {
	
// reserve �� ���� �ܿ��� ó��
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

	int add_ref( void );
	int dec_ref( void );

	void swap( buffer_base& rhs );

	void reserve( std::size_t sz );
private:
	// std::atomic<int> c++11 ������ �������� ����
	tdk::threading::atomic<int>* _counter(void);
private:
	void* _base;
	std::size_t _size;
	tdk::buffer::allocator* _allocator;
};

}}

#endif