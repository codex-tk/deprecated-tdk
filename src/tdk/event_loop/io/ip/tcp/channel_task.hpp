/*
 * channel_task.hpp
 *
 *  Created on: 2013. 10. 10.
 *      Author: tk
 */

#ifndef CHANNEL_TASK_HPP_
#define CHANNEL_TASK_HPP_

#include <tdk/event_loop/io_task.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {
class channel;
class channel_task: public tdk::io_task {
public:
	channel_task();
	channel_task( tdk::task::handler h , void* ctx );
	~channel_task();
	tdk::task* thread_task( void );
	tdk::io::ip::tcp::channel* channel( void );
	void channel( tdk::io::ip::tcp::channel* chan );
private:
	tdk::task _thread_task;
	tdk::io::ip::tcp::channel* _channel;
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* CHANNEL_TASK_HPP_ */
