#include "stdafx.h"
#include <tdk.task/network/tcp/channel.hpp>
#include <tdk.task/task/event_loop.hpp>
#include <tdk.task/task/win32/io_engine.hpp>
namespace tdk {
namespace network {
namespace tcp {

channel::channel( tdk::task::event_loop& loop )
	: event_loop_object( loop )
{
}

bool channel::open( tdk::network::socket& fd ) {
	_fd.close();
	_fd.handle( fd.handle());
	_fd.set_option( tdk::network::socket::option::non_blocking());
	return loop().engine().bind( _fd.handle() , this );
}

void channel::close( void ) {
	if ( loop().current() == &loop() ) {
		_fd.close();
	} else {
		loop().post( [this]() {
			close();
		});
	}
}

void channel::recv( recv_operation* op  ) {
	loop().engine().add_recv_io( op );
}

void channel::recv( recv_operation* op , int size ) {
	loop().engine().add_recv_io( op , size );
}

void channel::send( send_operation* op ) {
	loop().engine().add_send_io( op );
}

tdk::network::socket& channel::socket( void ) {
	return _fd;
}


}}}