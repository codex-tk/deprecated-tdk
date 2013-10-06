#ifndef __tdk_io_task_h__
#define __tdk_io_task_h__

#include <system_error>
#include <tdk/event_loop/task.hpp>

namespace tdk {

class io_task : public task {
public:
	io_task( void );
	io_task( task::handler h , void* ctx = nullptr );

	~io_task( void );

	const std::error_code& error( void ) ;
    void error( const std::error_code& ec ) ;

    int io_bytes( void );
    void io_bytes( int io );
private:
	std::error_code _error;
	int _io_bytes;
};

}

#endif