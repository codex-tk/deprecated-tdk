/*
 * channel.cpp
 *
 *  Created on: 2013. 10. 22.
 *      Author: tk
 */

#include <tdk/io/ip/tcp/channel.hpp>
#include <tdk/io/ip/tcp/pipeline/filter.hpp>
#include <tdk/io/buffer_adapter.hpp>
#include <tdk/event_loop/event_loop.hpp>
#include <tdk/error/error_platform.hpp>
#include <tdk/error/error_tdk.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

namespace detail {
	int k_error_bit = 0x01;
	int k_close_bit = 0x02;
}

channel::channel(tdk::event_loop& loop , int fd )
	: _loop( loop )
	, _socket(fd)
	, _io_context( &channel::_handle_io_events , this )
	, _on_send( &channel::_handle_send , this )
	, _do_close( &channel::_handle_close , this )
	, _do_error_proagation( &channel::_handle_error_proagation , this )
	, _pipeline(this)
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

void channel::write( tdk::buffer::memory_block& msg ){
	retain();
	_loop.execute(task::make_one_shot_task(
						[this,msg]{
							fire_do_write(msg);
							release();
						}));
}

void channel::_register_handle(void) {
	if (_state.load() != 0)
		return;

	_io_context.evt( EPOLLIN);
	if (_loop.io_impl().register_handle(_socket.handle(), &_io_context)) {
		_loop.add_active();
	} else {
		_error_propagation(tdk::epoll_error( errno ));
	}
}


void channel::fire_on_connected(void) {
	retain();
	_register_handle();
	_pipeline.in_bound_filter()->on_connected();
}


void channel::fire_on_accepted( const tdk::io::ip::address& addr ) {
	retain();
	_register_handle();
	_pipeline.in_bound_filter()->on_accepted(addr);
}

void channel::fire_on_read( tdk::buffer::memory_block& msg ) {
	_pipeline.in_bound_filter()->on_read(msg);
}

void channel::fire_on_write( int writed , bool flushed ) {
	_pipeline.in_bound_filter()->on_write(writed,flushed);
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

void channel::fire_do_write( tdk::buffer::memory_block msg ) {
	if ( _state.load() != 0 )
		return;
	_pipeline.out_bound_filter()->do_write(msg);
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

tcp::pipeline& channel::pipeline( void ) {
	return _pipeline;
}

tdk::io::ip::socket& channel::socket_impl( void ) {
	return _socket;
}

void channel::do_write(tdk::buffer::memory_block& msg){
	bool process = _send_queue.empty();
	_send_queue.push_back(msg);
	if (!process)
		return;

	if ( _send_remains()) {
		if (!_send_queue.empty()) {
			_io_context.evt(EPOLLIN | EPOLLOUT);
			if (!_loop.io_impl().register_handle(_socket.handle(), &_io_context)) {
				_on_send.error( tdk::epoll_error( errno ) );
			}
		}
	}
	retain();
	_loop.execute( &_on_send );
}

void channel::_handle_readable( void ) {
	tdk::buffer::memory_block msg(channel_config().recv_buffer_size );
	int readsize = 0;
	do {
		iovec buf;
		buf.iov_base = msg.wr_ptr();
		buf.iov_len  = channel_config().recv_buffer_size;
		readsize = readv( _socket.handle() , &buf , 1);
	} while (( readsize < 0) && ( errno == EINTR ));

	if ( readsize < 0 ) {
		_error_propagation( tdk::platform::error());
	} else if ( readsize == 0 ){
		_error_propagation( tdk::tdk_network_remote_closed );
	} else {
		msg.wr_ptr(readsize);
		fire_on_read(msg);
	}
}

void channel::_handle_writeable( void ) {
	if ( _state.load() != 0 )
		return;

	if (_send_remains()) {
		if (_send_queue.empty()) {
			_io_context.evt(EPOLLIN);
		} else {
			_io_context.evt(EPOLLIN | EPOLLOUT);
		}
		if (!_loop.io_impl().register_handle(_socket.handle(), &_io_context)) {
			_on_send.error(tdk::epoll_error( errno ));
		}
	}
	if ( _on_send.error() ) {
		_error_propagation(_on_send.error());
	} else {
		fire_on_write( _on_send.io_bytes() , _send_queue.empty() );
	}
}


bool channel::_send_remains( void ) {
	_on_send.error( std::error_code());
	_on_send.io_bytes(0);
	tdk::io::buffer_adapter buf;
	int total_write = 0;
	while ( !_send_queue.empty()) {
		for( auto it : _send_queue ) {
			if ( !buf.push_back( it.rd_ptr() , it.length())){
				break;
			}
		}
		int writesize = 0;
		do {
			writesize = writev( _socket.handle()
				, buf.buffers()
				, buf.count());
		} while (( writesize < 0)  && ( errno == EINTR ));
		if ( writesize < 0 ) {
			if ( errno == EAGAIN || errno == EWOULDBLOCK ) {
				_on_send.io_bytes( total_write );
				return true;
			}
			_on_send.error( tdk::platform::error());
			return false;
		} else {
			total_write += writesize;
			while ( writesize > 0 ) {
				auto msg = _send_queue.begin();
				int move = msg->rd_ptr(writesize);
				if (msg->length() == 0 )
					_send_queue.pop_front();
				writesize -= move;
			}
		}
	}
	_on_send.io_bytes( total_write );
	return true;
}

void channel::_handle_io_events( tdk::task* t ) {
	channel* c = static_cast< channel* >( t->context());
	c->handle_io_events();
}

void channel::handle_io_events( void ) {
	if ( _state.load() != 0 )
		return;

	int evt = _io_context.evt();
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
			if ( evt & EPOLLOUT ) {
				_handle_writeable();
			}
		}
	}
}

void channel::_handle_send( tdk::task* t ) {
	channel* c = static_cast< channel* >( t->context());
	c->handle_send();
	c->release();
}

void channel::handle_send( void ) {
	if ( _state.load() != 0  )
		return;
	if ( _on_send.error() ) {
		_error_propagation( _on_send.error());
	} else {
		fire_on_write( _on_send.io_bytes()
				, _send_queue.empty());
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

config& channel::channel_config( void ) {
	static config cfg(4096);
	return cfg;
}


} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
