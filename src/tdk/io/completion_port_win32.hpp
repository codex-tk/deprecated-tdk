#ifndef __tdk_io_completion_port_h__
#define __tdk_io_completion_port_h__

#include <tdk/time/date_time.hpp>

namespace tdk {
namespace io {
	
class completion_port {
public:
	typedef void (*callback)( 
		const tdk::error_code& code 
		, int io_byte 
		, void* key
		, OVERLAPPED* ov
		, void* ctx );
public:
	completion_port( void );
	~completion_port( void );

	bool create( int concurrent_threads = 0 );
	void close( void );
	
	int wait( const tdk::time_span& w
		, callback cb 
		, void* ctx );

	bool bind( SOCKET fd , void* key );

	bool post( int io_byte , void* key , OVERLAPPED* ov );	
private:
	HANDLE _handle;
};

}}

#endif