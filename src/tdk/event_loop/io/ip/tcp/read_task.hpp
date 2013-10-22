/*
 * read_task.hpp
 *
 *  Created on: 2013. 10. 10.
 *      Author: tk
 */

#ifndef READ_TASK_HPP_
#define READ_TASK_HPP_

#include <tdk/io/buffer_adapter.hpp>
#include <tdk/event_loop/io/ip/tcp/socket_task.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {
class channel;

class read_task: public socket_task {
public:
	read_task( void );
	read_task( tdk::task::handler h , void* ctx );
	~read_task( void );

	void buffers( const tdk::io::buffer_adapter& buf );

	tdk::io::buffer_adapter& buffers( void );
private:
	tdk::io::buffer_adapter _buffer;
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* READ_TASK_HPP_ */
