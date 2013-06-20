#include "stdafx.h"
#include <tdk/log/writer/udp_writer.hpp>
#include <tdk/log/record.hpp>

namespace tdk {
namespace log {

udp_writer::udp_writer( const tdk::io::ip::address& endpoint 
					   , const tdk::log::formatter_ptr& fmt )
	: writer(fmt) , _endpoint( endpoint ) 
{
	
}

udp_writer::~udp_writer( void ) {

}

void udp_writer::write( const record& r ){
	if ( _socket == tdk::io::ip::socket::invalid_socket() ) {
		_socket.open( AF_INET , SOCK_DGRAM , IPPROTO_UDP );
	}
	if ( _socket != tdk::io::ip::socket::invalid_socket() ) {
		_formatter->format( r , _buffer );
		_socket.sendto( _buffer.rd_ptr() , _buffer.length() , _endpoint );
		_buffer.clear();
	}
}

}}
