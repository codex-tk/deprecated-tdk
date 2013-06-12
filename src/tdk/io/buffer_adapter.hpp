#ifndef __tdk_io_buffer_adapter_h__
#define __tdk_io_buffer_adapter_h__

namespace tdk {
namespace io {

class buffer_adapter {
public:
	// TODO : 변환할 생성자 추가할것
	buffer_adapter( void )
		: _count(0){
	}

	buffer_adapter( const buffer_adapter& rhs ) 
		: _count( rhs._count )
	{
		memcpy( _buffers , rhs._buffers , k_buffer_size * sizeof( WSABUF ));
	}

	buffer_adapter( void* p , int size ) 
		: _count(1) 
	{
		_buffers[0].buf = (CHAR*)p;
		_buffers[0].len = size;
	}

	buffer_adapter& operator=( const buffer_adapter& rhs ) {
		_count = rhs._count;
		memcpy( _buffers , rhs._buffers , k_buffer_size * sizeof( WSABUF ));
		return *this;
	}

	~buffer_adapter( void ) {
	}

	const WSABUF* buffers( void ) const {
		return _buffers;
	}

	int count( void ) const {
		return _count;
	}

	int size( void ) const {
		int size = 0;
		for ( int i = 0 ; i < _count ; ++i ) {
			size += _buffers[i].len;
		}
		return size;
	}
private:
	static const int k_buffer_size = 64;
	WSABUF _buffers[k_buffer_size];
	int _count;
};

}}

#endif