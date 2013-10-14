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

	const std::error_code& error( void ) ;
    void error( const std::error_code& ec ) ;

    int io_bytes( void );
    void io_bytes( int io );
private:
	std::error_code _error;
	int _io_bytes;
};

} /* namespace io */
} /* namespace tdk */

#endif /* TASK_HPP_ */
