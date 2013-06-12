#include "stdafx.h"
#include <tdk.task/network/udp/channel.hpp>
#include <tdk.task/task/event_loop.hpp>
#include <tdk.task/task/win32/io_engine.hpp>

namespace tdk {
namespace network {
namespace udp {

channel::channel( tdk::task::event_loop& loop )
	: event_loop_object( loop )
{
}

bool channel::open( int af ) {
	if (_fd.open( af , SOCK_DGRAM , IPPROTO_UDP )){
		_fd.set_option( tdk::network::socket::option::non_blocking());
		return loop().engine().bind( _fd.handle() , this );
	}
	return false;
}

bool channel::bind( const tdk::network::address& addr ) {
	return _fd.bind( addr );
}

void channel::close( void ) {
	_fd.close();
}

tdk::network::socket& channel::socket( void ) {
	return _fd;
}

int channel::sendto( const tdk::buffer::memory_block& mb 
					 , const tdk::network::address& address ) 
{
	return _fd.sendto( mb.rd_ptr() , mb.length() , address );
}

int channel::sendto( void* data , int sz , const tdk::network::address& addr ) {
	return _fd.sendto( data , sz , addr );
}

void channel::recvfrom( recvfrom_operation* op ) {
	loop().engine().add_recvfrom_io( op );
}


}}}