/*
 * filter.cpp
 *
 *  Created on: 2013. 10. 18.
 *      Author: tk
 */
#include "stdafx.h"
#include <tdk/io/ip/tcp/pipeline/filter.hpp>
#include <tdk/io/ip/tcp/channel.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

filter::filter()
	: _channel(nullptr)
	, _in_bound(nullptr)
	, _out_bound(nullptr)
{
	// TODO Auto-generated constructor stub

}

filter::~filter() {
	// TODO Auto-generated destructor stub
}

tcp::channel* filter::channel( void ) {
	return _channel;
}

void filter::channel( tcp::channel* c ) {
	_channel = c;
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

void filter::on_read( tdk::buffer::memory_block& msg ) {
	if ( _in_bound ) {
		_in_bound->on_read( msg );
	}
}

void filter::on_closed(void) {
	if ( _in_bound ) {
		_in_bound->on_closed();
	}
}

void filter::do_write( tdk::buffer::memory_block& msg ) {
	write_out_bound(msg);
}

void filter::on_write( int write , bool flushed ){
	if ( _in_bound ) {
		_in_bound->on_write(write,flushed);
	}
}

void filter::write_out_bound( tdk::buffer::memory_block& msg ) {
	if ( _out_bound ) {
		_out_bound->do_write( msg );
	} else {
		_channel->do_write( msg );
	}
}

void filter::on_delete( void ) {
	delete this;
}

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
