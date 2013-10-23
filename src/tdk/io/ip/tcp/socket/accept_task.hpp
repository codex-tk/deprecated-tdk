/*
 * accept_task.hpp
 *
 *  Created on: 2013. 10. 15.
 *      Author: tk
 */

#ifndef ACCEPT_TASK_HPP_
#define ACCEPT_TASK_HPP_

#include <tdk/io/task.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class accept_task: public tdk::io::task {
public:
	accept_task();
	accept_task( tdk::task::handler h , void* ctx );
	~accept_task();

	int accepted( void );
	void accepted( int fd );
private:
	int _fd;
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* ACCEPT_TASK_HPP_ */
