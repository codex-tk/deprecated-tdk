#ifndef __tdk_task_win32_io_completion_port_h__
#define __tdk_task_win32_io_completion_port_h__

#include <tdk.task/task/operation.hpp>
#include <tdk/time/date_time.hpp>

namespace tdk {
namespace task {
class io_engine;
class io_completion_port {
public:
	typedef void (*complete_callback)( const tdk::error_code& code 
		, int io_byte 
		, void* key
		, OVERLAPPED* ov
		, void* ctx );
public:
	io_completion_port ( void );
	~io_completion_port ( void );

	bool open( complete_callback cb , void* ctx , int concurrent_thread = 0 );
	void close( void );

	bool bind( SOCKET fd , void* object );
	void unbind( SOCKET fd );

	HANDLE handle(void) const;

	bool post( int byte , void* key , OVERLAPPED* ov );
	//bool post( tdk::task::operation* op );

	int wait( const tdk::time_span& t );
private:
	complete_callback _callback;
	void* _ctx;
	HANDLE _handle;
};

}}

#endif