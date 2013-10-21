/*
 * pipeline_acceptor.hpp
 *
 *  Created on: 2013. 10. 21.
 *      Author: tk
 */

#ifndef PIPELINE_ACCEPTOR_HPP_
#define PIPELINE_ACCEPTOR_HPP_
#include <tdk/io/ip/address.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class pipeline_acceptor {
public:
	pipeline_acceptor();
	virtual ~pipeline_acceptor();

	virtual bool condition( const tdk::io::ip::address& addr );
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* PIPELINE_ACCEPTOR_HPP_ */
