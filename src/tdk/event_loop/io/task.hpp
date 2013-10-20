/*
 * task.hpp
 *
 *  Created on: 2013. 10. 14.
 *      Author: tk
 */

#ifndef TASK_HPP_
#define TASK_HPP_

#include <system_error>
#include <tdk/event_loop/task.hpp>

namespace tdk {
namespace io {

class task : public tdk::task {
public:
	task( void );
	task( tdk::task::handler h , void* ctx = nullptr );

	~task( void );

	std::error_code error( void ) ;
    void error( const std::error_code& ec ) ;

    int io_bytes( void );
    void io_bytes( int io );
#if defined (_WIN32)
	struct overlapped_ex : public OVERLAPPED {
		io::task* task_ptr;
	};
	OVERLAPPED* impl( void );

	void reset( void );
#endif
private:
#if defined (_WIN32)
	
	overlapped_ex _impl;
#else
	std::error_code _error;
	int _io_bytes;
#endif
};

} /* namespace io */
} /* namespace tdk */

#endif /* TASK_HPP_ */
