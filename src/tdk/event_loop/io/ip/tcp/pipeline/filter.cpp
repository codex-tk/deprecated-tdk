/*
 * filter.cpp
 *
 *  Created on: 2013. 10. 18.
 *      Author: tk
 */
#include "stdafx.h"
#include <tdk/event_loop/io/ip/tcp/pipeline/filter.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

filter::filter()
	: _pipeline(nullptr)
	, _prev(nullptr)
	, _next(nullptr)
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

void filter::bind_filter( filter* prev , filter* next ) {
	_prev = prev;
	_next = next;
}

filter* filter::prev( void ) {
	return _prev;
}

filter* filter::next( void ) {
	return _next;
}

void filter::send( const std::vector< message >& msg ) {
	if ( prev() != nullptr ) {
		prev()->send( msg );
	}
}

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
