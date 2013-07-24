#include "stdafx.h"
#include <tdk/io/buffer_adapter.hpp>
#include <string.h>

namespace tdk {
namespace io {

    buffer_adapter::buffer_adapter( void )
		: _count(0){
	}

	buffer_adapter::buffer_adapter( const buffer_adapter& rhs ) 
		: _count( rhs._count )
	{
		memcpy( _buffers , rhs._buffers , k_buffer_size * sizeof( buffer_type ));
	}

	buffer_adapter::buffer_adapter( void* p , int size ) 
		: _count(1) 
	{
#if defined( _WIN32 )        
		_buffers[0].buf = (CHAR*)p;
		_buffers[0].len = size;
#else
        _buffers[0].iov_base = p;
        _buffers[0].iov_len  = size;
#endif        
	}

	buffer_adapter& buffer_adapter::operator=( const buffer_adapter& rhs ) {
		_count = rhs._count;
		memcpy( _buffers , rhs._buffers , k_buffer_size * sizeof( buffer_type ));
		return *this;
	}

	buffer_adapter::~buffer_adapter( void ) {
	}

	const buffer_adapter::buffer_type* buffer_adapter::buffers( void ) const {
		return _buffers;
	}

	int buffer_adapter::count( void ) const {
		return _count;
	}

	int buffer_adapter::size( void ) const {
		int size = 0;
		for ( int i = 0 ; i < _count ; ++i ) {
#if defined( _WIN32 )
            size += _buffers[i].len;
#else
            size += _buffers[i].iov_len;
#endif            
		}
		return size;
	}

	bool buffer_adapter::push_back( void* p , int size ) {
		if ( _count >= k_buffer_size )
			return false;
#if defined( _WIN32 )        
		_buffers[_count].buf = (CHAR*)p;
		_buffers[_count].len = size;
#else
        _buffers[_count].iov_base = p;
        _buffers[_count].iov_len  = size;
#endif        
		++_count;
		return true;
	}
}}