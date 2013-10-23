/*
 * config.hpp
 *
 *  Created on: 2013. 10. 21.
 *      Author: tk
 */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

struct config {
	config( int rbs = 4096);
	int recv_buffer_size;
	static const config& default_config( void );
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* CONFIG_HPP_ */
