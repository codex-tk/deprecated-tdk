/*
 * pipeline_acceptor.cpp
 *
 *  Created on: 2013. 10. 21.
 *      Author: tk
 */
#include "stdafx.h"
#include <tdk/io/ip/tcp/channel_acceptor.hpp>
#include <tdk/io/ip/tcp/channel.hpp>
#include <tdk/io/ip/tcp/pipeline/pipeline_builder.hpp>
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
	_address = addr;
	if ( _address.family() == AF_INET6 )
		_fd.open( tdk::io::ip::socket::tcp_v6);
	else
		_fd.open( tdk::io::ip::socket::tcp_v4);

	if ( _fd == tdk::io::ip::socket::invalid_socket())
		return false;

	tdk::io::ip::socket::option::reuse_address reuse_addr(true);
	tdk::io::ip::socket::option::non_blocking non_block;

	if ( !_fd.set_option(non_block)) {
		_fd.close();
		return false;
	}
	
	if ( !_fd.set_option(reuse_addr)) {
		_fd.close();
		return false;
	}
	if ( !_fd.bind(addr)) {
		_fd.close();
		return false;
	}
	if ( !_fd.listen()) {
		_fd.close();
		return false;
	}
#if defined (linux) || defined( __linux__ )
	_on_accept.evt( EPOLLIN );
	if ( !_loop
			.io_impl()
			.register_handle(_fd.handle() , &_on_accept)) {
		_fd.close();
		return false;
	}
	_loop.add_active();
	
#elif defined( _WIN32 )
	if ( !_loop
			.io_impl()
			.register_handle(_fd.handle() , this)) {
		_fd.close();
		return false;
	}
	do_accept();
#endif
	return true;
}

#if defined( _WIN32 )
void channel_acceptor::do_accept( void ) {
	tdk::io::ip::socket accepted_fd;

	if ( _address.family() == AF_INET6 )
		accepted_fd.open( tdk::io::ip::socket::tcp_v6);
	else
		accepted_fd.open( tdk::io::ip::socket::tcp_v4);

	if ( accepted_fd == tdk::io::ip::socket::invalid_socket()) {
		_on_accept.error( tdk::platform::error());
		_loop.execute( &_on_accept );
		return;
	}

	tdk::io::ip::socket::option::non_blocking non_block;
	accepted_fd.set_option(non_block);

	_on_accept.reset();
	_on_accept.accepted_fd(accepted_fd.handle());

    DWORD dwBytes = 0;
    if ( AcceptEx(  _fd.handle()
					,_on_accept.accepted_fd() 
					,_on_accept.address() 
					, 0 
					, sizeof( sockaddr_storage ) 
					, sizeof( sockaddr_storage ) 
					, &dwBytes 
					, _on_accept.impl() ) == FALSE ) 
    {
        std::error_code ec = tdk::platform::error();
        if ( ec.value() != WSA_IO_PENDING ){
			_on_accept.error( tdk::platform::error());
			_loop.execute( &_on_accept );
	    }
    }
	_loop.add_active();
}
#endif
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
#if defined (linux) || defined( __linux__ )
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
			if ( !condition(addr)){
				::close( fd );
			} else {
				tdk::io::ip::tcp::channel* c =
								new tdk::io::ip::tcp::channel(
										channel_loop()
										, fd );
				tdk::io::ip::socket::option::non_blocking non_block;
				c->socket_impl().set_option( non_block );
				if ( _builder->build( c->pipeline()) ) {
					::close( fd );
					delete c;
				} else {
					c->loop()
						.execute( tdk::task::make_one_shot_task([c ,addr]
					{
						c->fire_on_accepted(addr);
					}));
				}
			}
		}
	}
#elif defined( _WIN32 )
	if ( _on_accept.error()) {
		::closesocket( _on_accept.accepted_fd());
	} else {
		struct sockaddr* remote , *local;
		INT remotelen , locallen;
		GetAcceptExSockaddrs(	_on_accept.address()  ,
								0 , 
								sizeof( sockaddr_storage ) ,
								sizeof( sockaddr_storage ) ,
								&local , 
								&locallen , 
								&remote , 
								&remotelen );
		tdk::io::ip::address local_addr( local , locallen );
		tdk::io::ip::address remote_addr( remote , remotelen );

		if ( !condition(remote_addr)){
			::closesocket( _on_accept.accepted_fd() );
		} else {
			tdk::io::ip::tcp::channel* c =
						new tdk::io::ip::tcp::channel(
								channel_loop()
								, _on_accept.accepted_fd() );
			if ( _builder->build( c->pipeline()) ) {
				::closesocket( _on_accept.accepted_fd() );
				delete c;
			} else {
				c->loop()
					.execute( tdk::task::make_one_shot_task([c ,remote_addr]
				{
					c->fire_on_accepted(remote_addr);
				}));
			}
		}		
	}
	do_accept();
#endif
}

void channel_acceptor::_on_accept_handler(tdk::task* t) {
	channel_acceptor* a = static_cast< channel_acceptor* >(t->context());
	a->on_accept_handler();
	a->_loop.remove_active();
}


#if defined( _WIN32 )

channel_acceptor::task::task( tdk::task::handler h , void* ctx  )
	: io::task( h , ctx ){
}

channel_acceptor::task::~task( void ) {

}

SOCKET channel_acceptor::task::accepted_fd( void ) {
	return _accepted_fd;
}

void channel_acceptor::task::accepted_fd( SOCKET fd ) {
	_accepted_fd = fd;
}

tdk::io::ip::address* channel_acceptor::task::address( void ) {
	return _address;
}

#endif

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

