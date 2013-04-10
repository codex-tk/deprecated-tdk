#ifndef __tdk_task_win32_io_engine_h__
#define __tdk_task_win32_io_engine_h__

#include <tdk/error/error_code.hpp>
#include <tdk.task/task/win32/io_completion_port.hpp>

namespace tdk{
namespace task {

class event_loop;
class io_engine {
public:
	
public:
	io_engine( event_loop& loop );
	~io_engine( void );

	void on_complete( 
		const tdk::error_code& code 
		, int io_byte 
		, void* key
		, OVERLAPPED* ov );
private:
	event_loop& _loop;
	io_completion_port _port;
};

}}

#endif