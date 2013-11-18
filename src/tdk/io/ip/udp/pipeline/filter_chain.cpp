/*
 * filter_chain.cpp
 *
 *  Created on: 2013. 10. 21.
 *      Author: tk
 */
#include "stdafx.h"
#include <tdk/io/ip/udp/pipeline/filter_chain.hpp>
#include <tdk/io/ip/udp/pipeline/filter.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace udp {

filter_chain::filter_chain()
	: _head( nullptr )
	, _tail( nullptr )
{

}

filter_chain::~filter_chain() {
	while ( _head != nullptr ) {
		filter* d = _head;
		_head = _head->in_bound();
		d->on_delete();
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

udp::filter* filter_chain::in_bound_filter( void ) {
	return _head;
}

udp::filter* filter_chain::out_bound_filter( void ) {
	return _tail;
}

}}}}
