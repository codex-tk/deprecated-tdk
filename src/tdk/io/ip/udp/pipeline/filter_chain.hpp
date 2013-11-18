/*
 * filter_chain.hpp
 *
 *  Created on: 2013. 10. 21.
 *      Author: tk
 */

#ifndef UDP_FILTER_CHAIN_HPP_
#define UDP_FILTER_CHAIN_HPP_

namespace tdk {
namespace io {
namespace ip {
namespace udp {

class filter;

class filter_chain {
public:
	filter_chain();
	~filter_chain();

	void add( filter* f );

	filter* in_bound_filter( void );
	filter* out_bound_filter( void );
private:
	filter* _head;
	filter* _tail;
};

} /* namespace udp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* FILTER_CHAIN_HPP_ */
