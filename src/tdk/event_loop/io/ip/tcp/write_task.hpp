/*
 * write_task.hpp
 *
 *  Created on: 2013. 10. 10.
 *      Author: tk
 */

#ifndef WRITE_TASK_HPP_
#define WRITE_TASK_HPP_

#include <tdk/io/buffer_adapter.hpp>
#include <tdk/event_loop/io/ip/tcp/socket_task.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class write_task: public socket_task {
public:
	write_task();
	write_task( tdk::task::handler h , void* ctx );
	~write_task();

	void buffers( const tdk::io::buffer_adapter& buf );
	tdk::io::buffer_adapter& buffers( void );
	tdk::io::buffer_adapter remain_buffers( void );
private:
	tdk::io::buffer_adapter _buffer;
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* WRITE_TASK_HPP_ */
