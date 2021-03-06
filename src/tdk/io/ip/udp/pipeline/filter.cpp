/*
 * filter.cpp
 *
 *  Created on: 2013. 10. 18.
 *      Author: tk
 */
#include "stdafx.h"
#include <tdk/io/ip/udp/pipeline/filter.hpp>
#include <tdk/io/ip/udp/channel.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace udp {

filter::filter()
	: _channel(nullptr)
	, _in_bound(nullptr)
	, _out_bound(nullptr)
{
	
}

filter::~filter() {

}

udp::channel* filter::channel( void ) {
	return _channel;
}

void filter::channel( udp::channel* c ) {
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

void filter::on_open( void ) {
	if ( _in_bound ) {
		_in_bound->on_open();
	}
}

void filter::on_error( const std::error_code& ec ) {
	if ( _in_bound ) {
		_in_bound->on_error( ec );
	}
}

void filter::on_read( const tdk::io::ip::address& addr 
					 , tdk::buffer::memory_block& msg ) {
	if ( _in_bound ) {
		_in_bound->on_read( addr , msg );
	}
}

void filter::on_closed(void) {
	if ( _in_bound ) {
		_in_bound->on_closed();
	}
}

void filter::do_write( const tdk::io::ip::address& addr 
					  , tdk::buffer::memory_block& msg ) {
	write_out_bound( addr , msg);
}

void filter::write_out_bound( const tdk::io::ip::address& addr 
							 , tdk::buffer::memory_block& msg ) {
	if ( _out_bound ) {
		_out_bound->do_write(addr, msg );
	} else {
		_channel->do_write(addr, msg );
	}
}

void filter::on_delete( void ) {
	delete this;
}

const std::string& filter::name( void ) {
	return _name;
}

void filter::name( const std::string& n ) {
	_name = n;
}
}
}
}
}
