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

	return loop().engine().bind( _fd.handle() , this );
}

void channel::close( void ) {
	_fd.close();
}

void channel::recv( tdk::buffer::memory_block& mb ) {
	loop().engine().add_recv_io( *this , mb );
}

tdk::network::socket& channel::socket( void ) {
	return _fd;
}

}}}