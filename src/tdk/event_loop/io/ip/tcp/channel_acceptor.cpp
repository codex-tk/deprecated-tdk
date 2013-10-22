/*
 * pipeline_acceptor.cpp
 *
 *  Created on: 2013. 10. 21.
 *      Author: tk
 */

#include <tdk/event_loop/io/ip/tcp/channel_acceptor.hpp>
#include <tdk/event_loop/io/ip/tcp/channel.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline_builder.hpp>
#include <tdk/error/error_platform.hpp>
#include <tdk/error/error_tdk.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

channel_acceptor::channel_acceptor(event_loop& l )
	:_loop(l)
	, _builder( nullptr )
	, _on_accept( channel_acceptor::_on_accept_handler , this )
{
	// TODO Auto-generated constructor stub

}

channel_acceptor::~channel_acceptor() {
	// TODO Auto-generated destructor stub
}

bool channel_acceptor::condition( const tdk::io::ip::address& ) {
	return true;
}

event_loop& channel_acceptor::channel_loop( void ) {
	return _loop;
}

bool channel_acceptor::open( const tdk::io::ip::address& addr
		, pipeline_builder* builder )
{
	_builder = builder;

	if ( addr.family() == AF_INET6 )
		_fd.open( tdk::io::ip::socket::tcp_v6);
	else
		_fd.open( tdk::io::ip::socket::tcp_v4);

	if ( _fd == tdk::io::ip::socket::invalid_socket())
		return false;

	tdk::io::ip::socket::option::reuse_address reuse_addr(true);
	tdk::io::ip::socket::option::non_blocking non_block;

	if ( !_fd.set_option(non_block))
		goto acceptor_error;
	if ( !_fd.set_option(reuse_addr))
		goto acceptor_error;
	if ( !_fd.bind(addr))
		goto acceptor_error;
	if ( !_fd.listen())
		goto acceptor_error;
	_on_accept.evt( EPOLLIN );
	if ( !_loop
			.io_impl()
			.register_handle(_fd.handle() , &_on_accept))
		return false;
	_loop.add_active();
	return true;
acceptor_error:
	_fd.close();
	return false;
}

void channel_acceptor::close( void ) {
	if ( _loop.in_loop()) {
		if ( _fd == tdk::io::ip::socket::invalid_socket())
			return ;
		_fd.close();
		_loop.remove_active();
	} else {
		_loop.execute( tdk::task::make_one_shot_task([this]{ this->close(); }));
	}
}

void channel_acceptor::on_accept_handler( void ) {
	int evt = _on_accept.evt();
	if ( evt & EPOLLIN ) {
		while ( true ) {
			tdk::io::ip::address addr;
			SOCKET fd = -1;
			do {
				fd = ::accept( _fd.handle()
						, addr.sockaddr()
						, addr.sockaddr_length_ptr() );
			} while((fd==-1)&&(errno==EINTR));
			if ( fd < 0 )
				return;
			if ( condition(addr)){
				tdk::io::ip::tcp::channel* c =
								new tdk::io::ip::tcp::channel(
										channel_loop()
										, fd );
				if ( _builder->build( c->pipeline()) ) {
					delete c;
				} else {
					c->fire_on_accepted(addr);
					return;
				}
			}
			::close( fd );
		}
	}
}

void channel_acceptor::_on_accept_handler(tdk::task* t) {
	channel_acceptor* a = static_cast< channel_acceptor* >(t->context());
	a->on_accept_handler();
}

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
