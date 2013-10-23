/*
 * acceptor.cpp
 *
 *  Created on: 2013. 10. 15.
 *      Author: tk
 */

#include <tdk/io/ip/tcp/socket/acceptor.hpp>
#include <tdk/error/error_platform.hpp>
#include <tdk/error/error_tdk.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

acceptor::acceptor( tdk::event_loop& loop )
	: _acceptor_task( this , &acceptor::handle_event )
	, _loop( &loop )
{

}

acceptor::~acceptor() {
}

void acceptor::handle_event( int evt ) {
	_on_accept->error(std::error_code());
	_on_accept->accepted(-1);
	if ( evt & EPOLLERR ) {
		std::error_code ec = tdk::tdk_epoll_error;
		tdk::io::ip::socket::option::error sock_error;
		if ( _socket.get_option( sock_error ) && sock_error.value()!= 0){
			ec = tdk::platform::error( sock_error.value());
		}
		_on_accept->error(ec);
		_on_accept->accepted(-1);
	} else {
		if ( evt & EPOLLHUP ) {
			_on_accept->error(tdk::tdk_epoll_hang_up);
			_on_accept->accepted(-1);
		} else {
			if ( evt & EPOLLIN ) {
				tdk::io::ip::address addr;
				SOCKET fd = -1;
				do {
					fd = ::accept( _socket.handle() , addr.sockaddr() , addr.sockaddr_length_ptr() );
				} while((fd==-1)&&(errno==EINTR));
				if ( fd > 0 ) {
					_on_accept->accepted(fd);
				} else {
					_on_accept->error( tdk::platform::error());
				}
			}
		}
	}
	(*_on_accept)();
}

bool acceptor::open( const tdk::io::ip::address& addr
		, tdk::io::ip::tcp::accept_task* on_accept ) {
	close();
	_on_accept = on_accept;
	if ( addr.family() == AF_INET6 )
		_socket.open( tdk::io::ip::socket::tcp_v6);
	else
		_socket.open( tdk::io::ip::socket::tcp_v4);

	if ( _socket == tdk::io::ip::socket::invalid_socket())
		return false;

	tdk::io::ip::socket::option::reuse_address reuse_addr(true);
	tdk::io::ip::socket::option::non_blocking non_block;

	struct acceptor_auto_close {
		acceptor_auto_close( acceptor* a )
			: _acceptor(a) {
		}
		~acceptor_auto_close( void ) {
			if ( _acceptor != nullptr )
				_acceptor->close();
		}
		void detach(void) {
			_acceptor =  nullptr;
		}
		acceptor* _acceptor;
	};

	acceptor_auto_close aac( this );

	if ( !_socket.set_option(non_block))
		return false;
	if ( !_socket.set_option(reuse_addr))
		return false;
	if ( !_socket.bind(addr))
		return false;
	if ( !_socket.listen())
		return false;
	_acceptor_task.evt( EPOLLIN );
	if ( !_loop->io_impl().register_handle(_socket.handle() , &_acceptor_task))
		return false;

	aac.detach();
	_loop->add_active();
	return true;
}

void acceptor::close( void ) {
	if ( _loop->in_loop()) {
		if ( _socket == tdk::io::ip::socket::invalid_socket())
			return ;
		_socket.close();
		_loop->remove_active();
	} else {
		_loop->execute( tdk::task::make_one_shot_task([this]{ this->close(); }));
	}
}

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
