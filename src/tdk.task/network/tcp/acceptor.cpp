#include "stdafx.h"
#include <tdk.task/network/tcp/acceptor.hpp>
#include <tdk.task/task/event_loop.hpp>

namespace tdk {
namespace network {
namespace tcp {

acceptor::acceptor( tdk::task::event_loop& loop )
	: event_loop_object( loop ){
}

bool acceptor::open( const tdk::network::address& addr ) {
	close();

	_address = addr;
	bool result = false;
	if ( addr.family() == AF_INET6 ) {
		result = _fd.open( tdk::network::socket::tcp_v6 );
	} else {
		result = _fd.open( tdk::network::socket::tcp_v4 );
	}
	if ( !result ) {
		return false;
	}
	tdk::network::socket::option::reuse_address reuse_option( TRUE );
	tdk::network::socket::option::non_blocking  nonblock_option;
	if ( !_fd.set_option( reuse_option ) ) {
		return false;
	}
	if ( !_fd.set_option( nonblock_option ) ) {
		return false;
	}
	if ( !_fd.bind( addr ) ) {
		return false;
	}
	if ( !loop().engine().bind( _fd.handle() , this )){
		return false;
	}
	tdk::network::socket::option::conditional_accept cont_accept_option(TRUE);
	if ( !_fd.set_option( cont_accept_option ) ){
		return false;
	}
	if ( !_fd.listen() ) {
		return false;
	}
	return true;
}

void acceptor::close( void ) {
	_fd.close();
}

void acceptor::accept( tdk::network::tcp::accept_operation* op ) {
	return loop().engine().add_accept_io( op );
}
/*
void acceptor::accept( void ) {
	return loop().engine().add_accept_io( *this );
}
*/
tdk::network::socket& acceptor::socket( void ) {
	return _fd;
}

const tdk::network::address& acceptor::address( void ) {
	return _address;
}

}}}
