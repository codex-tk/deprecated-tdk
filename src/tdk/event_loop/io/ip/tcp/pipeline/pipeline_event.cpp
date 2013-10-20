#include "stdafx.h"
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline_event.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

void error_event::error( const std::error_code& ec ) {
	_ec = ec;
}

std::error_code error_event::error( void ) {
	return _ec;
}

message::message( void ) 
	: _type(0)
{

}

message::message( int type , const tdk::buffer::memory_block& mb ) 
	: _type(0) , _mb( mb )
{

}

message::message( const message& m ) 
	:_type(m._type) , _mb( m._mb )
{

}

message& message::operator=( const message& m ) {
	_type = m._type;
	_mb = m._mb;
	return *this;
}

message::~message( void ) {

}

int message::type( void ) {
	return _type;
}

void message::type( int t ) {
	_type = t;
}

tdk::buffer::memory_block& message::data( void ) {
	return _mb;
}

void message::data( const tdk::buffer::memory_block& mb ) {
	_mb = mb;
}


std::vector< message >& recv_event::messages( void ) {
	return _messages;
}

void recv_event::clear( void ) {
	_messages.clear();
}

void recv_event::add_message( const message& m ) {
	_messages.push_back( m );
}

}}}}