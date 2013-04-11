#include "stdafx.h"
#include <tdk.task/network/tcp/operation/recv_operation.hpp>


namespace tdk {
namespace network {
namespace tcp {

recv_operation::recv_operation( 
	tdk::network::tcp::channel* c
	, tdk::buffer::memory_block& mb  )
	:_channel( c )
	, _buffer( mb )
{
}

recv_operation::~recv_operation( void ) {

}


tdk::network::tcp::channel* recv_operation::channel( void ) {
	return _channel;
}

tdk::buffer::memory_block& recv_operation::buffer( void ) {
	return _buffer;
}

void recv_operation::operator()( void ){
	if ( io_byte() > 0 ) {
		_buffer.wr_ptr( io_byte());
	}
}


}}}