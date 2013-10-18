/*
 * filter.hpp
 *
 *  Created on: 2013. 10. 18.
 *      Author: tk
 */

#ifndef FILTER_HPP_
#define FILTER_HPP_

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class pipeline;
class filter {
public:
	filter();
	virtual ~filter();

	tcp::pipeline* pipeline( void );
	void pipeline( tcp::pipeline* p );
private:
	tcp::pipeline* _pipeline;
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* FILTER_HPP_ */
