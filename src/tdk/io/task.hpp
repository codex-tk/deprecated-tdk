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

#if !defined( _WIN32 )
#include <sys/epoll.h>
#endif

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
	typedef union _data {
		io::task *ptr;
	}data_t;
	typedef struct _overlapped_ex : public OVERLAPPED {
		data_t data;
	}impl_type;

	void reset( void );
#else
	typedef epoll_event impl_type;
	int evt( void );
	void evt( int e );
#endif
	impl_type* impl( void );

private:
	impl_type _impl;
#if !defined (_WIN32)
	std::error_code _error;
	int _io_bytes;
#endif
};

} /* namespace io */
} /* namespace tdk */

#endif /* TASK_HPP_ */
