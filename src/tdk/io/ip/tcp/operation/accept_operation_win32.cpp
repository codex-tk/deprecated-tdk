#include "stdafx.h"
#include <tdk/io/ip/tcp/operation/accept_operation_win32.hpp>
#include <tdk/io/ip/tcp/socket_win32.hpp>
namespace tdk {
namespace io {
namespace ip {
namespace tcp {
accept_operation::accept_operation( tdk::io::operation::callback cb
		, tdk::io::ip::tcp::socket& listenfd
		, tdk::io::ip::tcp::socket& fd)
		: tdk::io::operation( cb )
		, _socket( &fd )
		, _listen_socket( &listenfd )
{

}

accept_operation::~accept_operation( void ) {

}

bool accept_operation::end_opearation( void ) {
	tdk::io::ip::tcp::socket::option::update_accept_context ctx( _listen_socket->handle() );
	_socket->set_option( ctx );
	if ( _socket->engine().bind( _socket->handle() ) == false ) {
		error( tdk::platform::error() );
	}
	/*
	struct sockaddr* remote , *local;
	INT remotelen , locallen;
	GetAcceptExSockaddrs(	address_ptr() ,
								0 , 
								sizeof( sockaddr_storage ) ,
								sizeof( sockaddr_storage ) ,
								&local , 
								&locallen , 
								&remote , 
								&remotelen );

	_address[0].set( local , locallen );
	_address[1].set( remote , remotelen );*/
	return true;
}

void accept_operation::process( void ) {
	_socket->engine().async_accept( this );
}

tdk::io::ip::tcp::socket& accept_operation::socket( void ) {
	return *_socket;
}

tdk::io::ip::tcp::socket& accept_operation::listen_socket( void ) {
	return *_listen_socket;
}
	
void* accept_operation::address_buffer( void ) {
	return _address_buffer;
}

}}}}