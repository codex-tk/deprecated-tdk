#ifndef __tdk_task_win32_io_completion_port_h__
#define __tdk_task_win32_io_completion_port_h__

#include <tdk.task/task/operation.hpp>
#include <tdk/time/date_time.hpp>

namespace tdk {
namespace task {
class io_engine;
class io_completion_port {
public:

public:
	io_completion_port ( io_engine& engine );
	~io_completion_port ( void );

	bool open( void );
	void close( void );

	bool bind( SOCKET fd , void* object );
	void unbind( SOCKET fd );

	HANDLE handle(void) const;

	bool post( int byte , void* key , OVERLAPPED* ov );
	//bool post( tdk::task::operation* op );

	int wait( const tdk::time_span& t );
private:
	io_engine& _engine;
	HANDLE _handle;
};

}}

#endif