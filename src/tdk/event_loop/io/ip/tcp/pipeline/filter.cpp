/*
 * filter.cpp
 *
 *  Created on: 2013. 10. 18.
 *      Author: tk
 */

#include <tdk/event_loop/io/ip/tcp/pipeline/filter.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

filter::filter()
	: _pipeline(nullptr)
{
	// TODO Auto-generated constructor stub

}

filter::~filter() {
	// TODO Auto-generated destructor stub
}

tcp::pipeline* filter::pipeline( void ) {
	return _pipeline;
}
void filter::pipeline( tcp::pipeline* p ) {
	_pipeline = p;
}

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
