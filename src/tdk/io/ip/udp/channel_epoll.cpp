/*
 * channel.cpp
 *
 *  Created on: 2013. 10. 22.
 *      Author: tk
 */

#include <tdk/io/ip/udp/channel.hpp>
#include <tdk/io/ip/udp/pipeline/filter.hpp>
#include <tdk/io/buffer_adapter.hpp>
#include <tdk/event_loop/event_loop.hpp>
#include <tdk/error/error_platform.hpp>
#include <tdk/error/error_tdk.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace udp {

namespace detail {
	int k_error_bit			= 0x01;
	int k_close_bit			= 0x02;
	int k_read_pending_bit	= 0x04;
}

channel::channel(tdk::event_loop& loop , int fd )
	: _loop( loop )
	, _socket(fd)
	, _io_context( &channel::_handle_io_events , this )
	, _do_close( &channel::_handle_close , this )
	, _do_error_proagation( &channel::_handle_error_proagation , this )
	, _pipeline(this)
	, _reg_events(0)
{
	_state.store(0);
	_ref_count.store(0);
}

channel::~channel() {

}

tdk::event_loop& channel::loop( void ) {
	return _loop;
}

void channel::close( void ) {
	if ( _state.fetch_or( detail::k_close_bit) & detail::k_close_bit )
			return;
	// process next turn
	retain();
	_loop.execute( &_do_close );
}

void channel::write( const tdk::io::ip::address& addr
		, tdk::buffer::memory_block& msg ){
	retain();
	_loop.execute(task::make_one_shot_task(
						[this,addr,msg]{
							fire_do_write(addr,msg);
							release();
						}));
}


void channel::pending_read( void ) {
	_state |= detail::k_read_pending_bit;
}

void channel::continue_read( void ) {
	int old = _state.fetch_xor( ~detail::k_read_pending_bit );
	if ( old & detail::k_read_pending_bit ) {
		retain();
		_loop.execute(task::make_one_shot_task(
						[this]{
							_register_handle();
							release();
						}));
	}
}

bool channel::is_bits_on( int b ) {
	return ( _state.load() & b ) != 0;
}

void channel::_register_handle(void) {
	if (is_bits_on( detail::k_error_bit | detail::k_close_bit))
		return;

	int evt = EPOLLIN;
	if ( is_bits_on( detail::k_read_pending_bit )) {
		evt = 0;
	}

	if ( _reg_events != evt ) {
		_reg_events = evt;
		_io_context.evt( evt );
		if (!_loop.io_impl().register_handle(_socket.handle(), &_io_context)) {
			error_propagation(tdk::epoll_error( errno ));
		}
	}
}

void channel::fire_on_open(void) {
	retain();
	_register_handle();
	_loop.add_active();
	_pipeline.in_bound_filter()->on_open();
}

void channel::fire_on_read( const tdk::io::ip::address& addr
		, tdk::buffer::memory_block& msg  ) {
	_pipeline.in_bound_filter()->on_read(addr,msg);
}

void channel::fire_on_error( const std::error_code& ec ) {
	_socket.close();
	_pipeline.in_bound_filter()->on_error(ec);
}

void channel::fire_on_close( void ) {
	_socket.close();
	if ( (_state.fetch_or( detail::k_error_bit ) & detail::k_error_bit) == 0 ) {
		fire_on_error( tdk::tdk_user_abort );
	}
	_pipeline.in_bound_filter()->on_closed();
	_loop.remove_active();
	release();
}

void channel::fire_do_write(const tdk::io::ip::address& addr
		, tdk::buffer::memory_block msg)
{
	if (is_bits_on(detail::k_error_bit | detail::k_close_bit))
		return;
	_pipeline.out_bound_filter()->do_write(addr, msg);
}

void channel::_error_propagation(const std::error_code& err ) {
	if ( _state.fetch_or( detail::k_error_bit ) & detail::k_error_bit )
		return;
	fire_on_error(err);
}

void channel::error_propagation( const std::error_code& err  ) {
	if ( _state.fetch_or( detail::k_error_bit ) & detail::k_error_bit )
		return;
	// if user code req
	// process next turn
	retain();
	_do_error_proagation.error( err );
	_loop.execute( &_do_error_proagation );
}

udp::pipeline& channel::pipeline( void ) {
	return _pipeline;
}

tdk::io::ip::socket& channel::socket_impl( void ) {
	return _socket;
}

void channel::do_write(const tdk::io::ip::address& addr,
		tdk::buffer::memory_block& msg)
{
	if (is_bits_on(detail::k_error_bit | detail::k_close_bit))
		return;

	int sendsize = 0;
	do {
		sendsize = sendto( _socket.handle()
				, msg.rd_ptr()
				, msg.length()
				, 0
				, addr.sockaddr()
				, addr.sockaddr_length());
	}while((sendsize <0 ) &&(errno==EINTR));
	if ( sendsize <= 0 ) {
		_error_propagation( tdk::platform::error());
	}
}

void channel::_handle_readable( void ) {
	if ( is_bits_on( detail::k_error_bit
		| detail::k_close_bit
		| detail::k_read_pending_bit ))
		return;

	int mtu = 1440;
	tdk::buffer::memory_block msg( mtu );
	tdk::io::ip::address recv_addr;

	int readsize = 0;
	do {
		readsize = recvfrom( _socket.handle()
				, msg.wr_ptr()
				, mtu
				, 0
				, recv_addr.sockaddr()
				, recv_addr.sockaddr_length_ptr());
	} while (( readsize < 0) && ( errno == EINTR ));

	if ( readsize < 0 ) {
		_error_propagation( tdk::platform::error());
	} else if ( readsize == 0 ){
		_error_propagation( tdk::tdk_network_remote_closed );
	} else {
		msg.wr_ptr(readsize);
		fire_on_read( recv_addr ,  msg);
	}
}

void channel::_handle_io_events( tdk::task* t ) {
	channel* c = static_cast< channel* >( t->context());
	c->handle_io_events( c->_io_context.evt());
}

void channel::handle_io_events( int evt ) {
	if ( is_bits_on( detail::k_error_bit | detail::k_close_bit))
		return;

	if ( evt & EPOLLERR ) {
		std::error_code ec = tdk::tdk_epoll_error;
		tdk::io::ip::socket::option::error sock_error;
		if ( _socket.get_option( sock_error ) && sock_error.value()!= 0){
			ec = tdk::platform::error( sock_error.value());
		}
		_error_propagation( ec );
	} else {
		if ( evt & EPOLLHUP ) {
			_error_propagation(tdk::tdk_epoll_hang_up);
		} else {
			if ( evt & EPOLLIN ) {
				_handle_readable();
			}
			_register_handle();
		}
	}
}

void channel::_handle_close( tdk::task* t ) {
	channel* c = static_cast< channel* >( t->context());
	c->fire_on_close();
	c->release();
}

void channel::_handle_error_proagation( tdk::task* t ) {
	channel* c = static_cast< channel* >( t->context());
	tdk::io::task* impl = static_cast< tdk::io::task* >(t);
	c->fire_on_error( impl->error());
	c->release();
}


void channel::retain( void ) {
	++_ref_count;
}

void channel::release( void ) {
	if ( --_ref_count == 0 ) {
		delete this;
	}
}


} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
