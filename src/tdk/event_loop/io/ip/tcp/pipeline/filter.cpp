/*
 * filter.cpp
 *
 *  Created on: 2013. 10. 18.
 *      Author: tk
 */
#include "stdafx.h"
#include <tdk/event_loop/io/ip/tcp/pipeline/filter.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

filter::filter()
	: _pipeline(nullptr)
	, _in_bound(nullptr)
	, _out_bound(nullptr)
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

filter* filter::in_bound( void ) {
	return _in_bound;
}

filter* filter::out_bound( void ) {
	return _out_bound;
}

void filter::in_bound( filter* f ) {
	_in_bound = f;
}

void filter::out_bound( filter* f ) {
	_out_bound = f;
}

void filter::on_accepted( const tdk::io::ip::address& addr ) {
	if ( _in_bound ) {
		_in_bound->on_accepted(addr);
	}
}

void filter::on_connected( void ) {
	if ( _in_bound ) {
		_in_bound->on_connected();
	}
}

void filter::on_error( const std::error_code& ec ) {
	if ( _in_bound ) {
		_in_bound->on_error( ec );
	}
}

void filter::on_read( tcp::message& msg ) {
	if ( _in_bound ) {
		_in_bound->on_read( msg );
	}
}

void filter::on_closed(void) {
	if ( _in_bound ) {
		_in_bound->on_closed();
	}
}

void filter::on_write( tcp::message& msg ) {
	write_out_bound(msg);
}

void filter::write_out_bound( tcp::message& msg ) {
	if ( _out_bound ) {
		_out_bound->on_write( msg );
	} else {
		_pipeline->on_write( msg );
	}
}

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
