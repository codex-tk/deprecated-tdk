#include "stdafx.h"
#include <tdk.task/network/tcp/operation/accept_operation.hpp>


namespace tdk {
namespace network {
namespace tcp {

accept_operation::accept_operation( tdk::network::tcp::acceptor* a )
	: _acceptor( a )
{
}

accept_operation::~accept_operation( void ) {
}


tdk::network::socket& accept_operation::socket( void ) {
	return _fd;
}

sockaddr_storage* accept_operation::address_ptr( void ) {
	return _address;
}

tdk::network::tcp::acceptor* accept_operation::acceptor( void ) {
	return _acceptor;
}
}}}