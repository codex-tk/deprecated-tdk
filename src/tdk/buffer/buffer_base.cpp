#include "stdafx.h"
#include <tdk/buffer/buffer_base.hpp>

namespace tdk {
namespace buffer {
namespace detail {

tdk::buffer::allocator malloc_allocator;

}

buffer_base::buffer_base( std::size_t sz 
	, tdk::buffer::allocator* alloc ) 
	: _allocator( alloc )
	, _size( sz )
	, _base( nullptr )
{
	if ( _allocator == nullptr ) {
		_allocator = &detail::malloc_allocator;
	}
	_base = _allocator->alloc( sizeof( tdk::threading::atomic<int> ) + _size );
	new (_base) tdk::threading::atomic<int>;
	_counter()->store(0);
	add_ref();
}

buffer_base::buffer_base( void* base 
	, std::size_t sz 
	, tdk::buffer::allocator* alloc )
	: _allocator( alloc )
	, _size( sz )
	, _base( base )
{

}

buffer_base::~buffer_base( void ) {
	dec_ref();
}

buffer_base::buffer_base( const buffer_base& rhs )
	:_base ( rhs._base )
	,_size ( rhs._size )
	,_allocator ( rhs._allocator )
{
	add_ref();
}

buffer_base& buffer_base::operator=( const buffer_base& rhs ) {
	buffer_base _new( rhs );
	swap( _new );
	return *this;
}

int buffer_base::ref_count( void ) {
	tdk::threading::atomic<int>* c = _counter();
	if (c) 
		return _counter()->load();
	return 1;
}

uint8_t* buffer_base::data_ptr( void ) const {
	if ( _allocator ) {
		return static_cast< uint8_t* >( _base ) + sizeof( tdk::threading::atomic<int> );
	}
	return static_cast< uint8_t* >( _base );
}

std::size_t buffer_base::size( void ) const {
	return _size;
}

tdk::threading::atomic<int>* buffer_base::_counter(void) {
	if ( _allocator ) {
		return static_cast<tdk::threading::atomic<int>*>(_base);
	}
	return nullptr;
}

int buffer_base::add_ref( void ) {
	if ( _allocator ) {
		return _counter()->increment();
		//int old = _counter()->fetch_add(1);
		//return old + 1;
	}
	return 1;
}

int buffer_base::dec_ref( void ) {
	if ( _allocator ) {
		/*
		int old = _counter()->fetch_sub(1);
		if ( old == 1 ) {
			_counter()->~atomic<int>();
			_allocator->free( _base );
		}
		return old - 1;
		*/
		if ( _counter()->decrement()  == 0 ) {
			_allocator->free( _base );
			_allocator = nullptr;
			_base = nullptr;
		}
	}
	return 1;
}

void buffer_base::swap( buffer_base& rhs ) {
	std::swap( _base , rhs._base );
	std::swap( _size , rhs._size );
	std::swap( _allocator , rhs._allocator );
}

void buffer_base::reserve( std::size_t sz ) {
	if ( ref_count() == 1 && _size >= sz ) {
		return;
	}
	buffer_base new_buffer( sz , _allocator );
	memcpy( new_buffer.data_ptr() , data_ptr() , min( _size , sz ));
	swap( new_buffer );
	_size = sz;
}

}}