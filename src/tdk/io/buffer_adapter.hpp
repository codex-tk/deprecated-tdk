#ifndef __tdk_io_buffer_adapter_h__
#define __tdk_io_buffer_adapter_h__

#if defined( _WIN32 )
#else
#include <sys/uio.h>
#endif

namespace tdk {
namespace io {

class buffer_adapter {
public:
#if defined( _WIN32 )
    typedef WSABUF buffer_type;
#else
    typedef iovec  buffer_type;
#endif
	// TODO : 변환할 생성자 추가할것
	buffer_adapter( void );

	buffer_adapter( const buffer_adapter& rhs ) ;
	buffer_adapter( void* p , int size ) ;
    buffer_adapter& operator=( const buffer_adapter& rhs ) ;
    ~buffer_adapter( void ) ;

	const buffer_type* buffers( void ) const ;

	int count( void ) const ;

	int size( void ) const ;

	bool push_back( void* p , int size ) ;
private:
	static const int k_buffer_size = 64;
    buffer_type _buffers[k_buffer_size];
	int _count;
};

}}

#endif
