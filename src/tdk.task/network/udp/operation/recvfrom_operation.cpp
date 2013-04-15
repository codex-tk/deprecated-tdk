#include "stdafx.h"
#include <tdk.task/network/udp/operation/recvfrom_operation.hpp>
#include <tdk.task/network/udp/channel.hpp>

namespace tdk {
namespace network {
namespace udp {

recvfrom_operation::recvfrom_operation( tdk::network::udp::channel& c )
	: _channel(&c){

}

recvfrom_operation::recvfrom_operation( tdk::network::udp::channel& c
	, const tdk::buffer::memory_block& mb  )
	:_channel( &c )
	, _buffer( mb )
{
}

recvfrom_operation::~recvfrom_operation( void ) {

}


tdk::network::udp::channel& recvfrom_operation::channel( void ) {
	return *_channel;
}

tdk::buffer::memory_block& recvfrom_operation::buffer( void ) {
	return _buffer;
}

tdk::network::address& recvfrom_operation::address( void ) {
	return _address;
}

void recvfrom_operation::buffer( const tdk::buffer::memory_block& mb ) {
	_buffer = mb;
}

void recvfrom_operation::before_dispatch( void ) {
	if ( io_byte() > 0 ) {
		_buffer.wr_ptr( io_byte());
	}
}

void recvfrom_operation::after_dispatch( void ) {
}


}}}