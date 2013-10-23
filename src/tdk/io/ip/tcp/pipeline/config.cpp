/*
 * config.cpp
 *
 *  Created on: 2013. 10. 21.
 *      Author: tk
 */
#include "stdafx.h"
#include <tdk/io/ip/tcp/pipeline/config.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

config::config( int rbs )
	: recv_buffer_size(rbs) {

}

const config& config::default_config( void ) {
	static config cfg;
	return cfg;
}

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
