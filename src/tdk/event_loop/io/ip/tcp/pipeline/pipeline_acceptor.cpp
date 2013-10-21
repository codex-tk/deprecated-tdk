/*
 * pipeline_acceptor.cpp
 *
 *  Created on: 2013. 10. 21.
 *      Author: tk
 */

#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline_acceptor.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline_builder.hpp>
#include <tdk/error/error_platform.hpp>
#include <tdk/error/error_tdk.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

pipeline_acceptor::pipeline_acceptor(event_loop& l )
	:_loop(l)
	, _builder( nullptr )
	, _on_accept( pipeline_acceptor::_on_accept_handler , this )
{
	// TODO Auto-generated constructor stub

}

pipeline_acceptor::~pipeline_acceptor() {
	// TODO Auto-generated destructor stub
}

bool pipeline_acceptor::condition( const tdk::io::ip::address& ) {
	return true;
}

bool pipeline_acceptor::open( const tdk::io::ip::address& addr
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

	struct acceptor_auto_close {
		acceptor_auto_close( pipeline_acceptor* a )
			: _acceptor(a) {
		}
		~acceptor_auto_close( void ) {
			if ( _acceptor != nullptr )
				_acceptor->close();
		}
		void detach(void) {
			_acceptor =  nullptr;
		}
		pipeline_acceptor* _acceptor;
	};

	acceptor_auto_close aac( this );

	if ( !_fd.set_option(non_block))
		return false;
	if ( !_fd.set_option(reuse_addr))
		return false;
	if ( !_fd.bind(addr))
		return false;
	if ( !_fd.listen())
		return false;
	_on_accept.evt( EPOLLIN );
	if ( !_loop
			.io_impl()
			.register_handle(_fd.handle() , &_on_accept))
		return false;

	aac.detach();
	_loop.add_active();
	return true;
}

void pipeline_acceptor::close( void ) {
	if ( _loop.in_loop()) {
		if ( _fd == tdk::io::ip::socket::invalid_socket())
			return ;
		_fd.close();
		_loop.remove_active();
	} else {
		_loop.execute( tdk::task::make_one_shot_task([this]{ this->close(); }));
	}
}

void pipeline_acceptor::on_accept_handler( void ) {
	int evt = _on_accept.evt();
	if ( evt & EPOLLIN ) {
		tdk::io::ip::address addr;
		SOCKET fd = -1;
		do {
			fd = ::accept( _fd.handle() , addr.sockaddr() , addr.sockaddr_length_ptr() );
		} while((fd==-1)&&(errno==EINTR));
		if ( fd > 0 ) {
			if ( condition(addr)){
				tdk::io::ip::tcp::pipeline* p =
					new tdk::io::ip::tcp::pipeline(
							_loop
							, _builder->config()
							, fd );
				std::error_code ec = _builder->build( *p );
				if ( ec ) {
					delete p;
				} else {
					p->on_accepted(addr);
					return;
				}
			}
			::close( fd );
		}
	}
}

void pipeline_acceptor::_on_accept_handler(tdk::task* t) {
	pipeline_acceptor* a = static_cast< pipeline_acceptor* >(t->context());
	a->on_accept_handler();
}

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
