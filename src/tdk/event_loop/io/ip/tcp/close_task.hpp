/*
 * close_task.hpp
 *
 *  Created on: 2013. 10. 10.
 *      Author: tk
 */

#ifndef CLOSE_TASK_HPP_
#define CLOSE_TASK_HPP_

#include <tdk/event_loop/io/ip/tcp/channel_task.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class channel;
class close_task : public tdk::io::ip::tcp::channel_task {
public:
	close_task();
	close_task( tdk::task::handler h , void * ctx );
	~close_task();
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* CLOSE_TASK_HPP_ */
