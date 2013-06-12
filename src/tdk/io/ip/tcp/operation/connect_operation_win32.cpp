#include "stdafx.h"
#include <tdk/io/ip/tcp/operation/connect_operation_win32.hpp>
#include <tdk/io/ip/tcp/socket_win32.hpp>
namespace tdk {
namespace io {
namespace ip {
namespace tcp {

connect_operation::connect_operation(
	tdk::io::ip::tcp::socket& fd
	, const std::vector< tdk::io::ip::address >& addrs
	, tdk::io::operation::callback cb  ) 
	: tdk::io::operation( cb )
	, _socket( &fd )
	, _address( addrs )
{
	_current_end_point = _address.begin();
}

connect_operation::~connect_operation( void ) {

}

void connect_operation::process( void ) {
	_socket->engine().async_connect( this );
}

bool connect_operation::end_opearation( void ) {
	if ( error() ) {
		++_current_end_point;
		if ( _current_end_point == _address.end() ){
			return true;
		}
		_socket->close();
		process();
		return false;
	}
	return true;
}

tdk::io::ip::tcp::socket& connect_operation::socket( void ){
	return *_socket;
}


const tdk::io::ip::address& connect_operation::address( void ) {
	return *_current_end_point;
}

}}}}