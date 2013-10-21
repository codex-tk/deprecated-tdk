/*
 * filter_chain.cpp
 *
 *  Created on: 2013. 10. 21.
 *      Author: tk
 */

#include <tdk/event_loop/io/ip/tcp/pipeline/filter_chain.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/filter.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

filter_chain::filter_chain()
	: _head( nullptr )
	, _tail( nullptr )
{
	// TODO Auto-generated constructor stub

}

filter_chain::~filter_chain() {
	while ( _head != nullptr ) {
		filter* d = _head;
		_head = _head->in_bound();
		delete d;
	}
}

void filter_chain::add( filter* f ) {
	if ( _tail == nullptr ) {
		_head = _tail = f;
	} else {
		_tail->in_bound(f);
		f->out_bound( _tail );
		_tail = f;
	}
}

filter* filter_chain::inbound_filter( void ) {
	return _head;
}

filter* filter_chain::outbound_filter( void ) {
	return _tail;
}

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
