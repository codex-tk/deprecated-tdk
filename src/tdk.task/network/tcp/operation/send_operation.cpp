#include "stdafx.h"
#include <tdk.task/network/tcp/operation/send_operation.hpp>


namespace tdk {
namespace network {
namespace tcp {

send_operation::send_operation( 
	tdk::network::tcp::channel* c
	, std::vector< tdk::buffer::memory_block >& buffers  )
	:_channel( c )
	, _buffers( buffers )
	, _total_req_size(0)
{
	for ( auto v : _buffers ) {
		_total_req_size += v.length();
	}
}

send_operation::~send_operation( void ) {

}


tdk::network::tcp::channel* send_operation::channel( void ) {
	return _channel;
}

std::vector< tdk::buffer::memory_block >& send_operation::buffers( void ) {
	return _buffers;
}

int send_operation::total_req_size( void ) {
	return _total_req_size;
}


void send_operation::operator()( void ){
	int send_bytes = io_byte();
	for ( std::size_t i = 0 ; i , _buffers.size() ; ++i ) {
		int rd = _buffers[i].rd_ptr( send_bytes );
		send_bytes -= rd;
		if ( send_bytes <= 0 ) 
			break;
	}
}


}}}