#include "stdafx.h"
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/filter.hpp>
#include <tdk/event_loop/event_loop.hpp>
#include <tdk/error/error_platform.hpp>
#include <tdk/error/error_tdk.hpp>
#include <tdk/io/buffer_adapter.hpp>
namespace tdk {
namespace io {
namespace ip {
namespace tcp {

namespace detail {
	int ERROR_BIT = 0x01;
	int CLOSE_BIT	= 0x02;
}


pipeline::pipeline(
	tdk::event_loop& loop
	, const tdk::io::ip::tcp::config& cfg
	, int fd
)	: _loop(loop)
	, _config( cfg )
	, _socket( fd )
	, _io_context( &pipeline::_handle_io_events , this )
{
	_state.store(0);
}

pipeline::~pipeline( void ) {

}

tdk::event_loop& pipeline::loop( void ) {
	return _loop;
}

void pipeline::add( filter* f ) {
	_chain.add(f);
	f->pipeline( this );
}

void pipeline::on_connected(void) {
	_chain.inbound_filter()->on_connected();

	if ( _state.load() != 0 )
		return;

	_io_context.evt( EPOLLIN );
	if ( _loop.io_impl().register_handle(
				_socket.handle()
				, &_io_context ) )
	{
		_loop.add_active();
	} else {
		error_propagation( tdk::platform::error());
	}
}

void pipeline::_handle_io_events( tdk::task* t ) {
	pipeline* pline = static_cast< pipeline* >( t->context());
	pline->handle_io_events();
}

void pipeline::handle_io_events( void ) {
	if ( _state.load() != 0 )
		return;

	int evt = _io_context.evt();
	if ( evt & EPOLLERR ) {
		std::error_code ec = tdk::tdk_epoll_error;
		tdk::io::ip::socket::option::error sock_error;
		if ( _socket.get_option( sock_error ) && sock_error.value()!= 0){
			ec = tdk::platform::error( sock_error.value());
		}
		error_propagation( ec );
	} else {
		if ( evt & EPOLLHUP ) {
			error_propagation(tdk::tdk_epoll_hang_up);
		} else {
			if ( evt & EPOLLIN ) {
				handle_readable();
			}
			if ( evt & EPOLLOUT ) {
				handle_writeable();
			}
		}
	}
}

void pipeline::handle_readable( void ) {
	tcp::message msg( 0 , _config.recv_buffer_size );
	int readsize = 0;
	do {
		iovec buf;
		buf.iov_base = msg.data().wr_ptr();
		buf.iov_len  = _config.recv_buffer_size;
		readsize = readv( _socket.handle() , &buf , 1);
	} while (( readsize < 0) && ( errno == EINTR ));

	if ( readsize < 0 ) {
		error_propagation( tdk::platform::error());
	} else if ( readsize == 0 ){
		error_propagation( tdk::tdk_network_remote_closed );
	} else {
		msg.data().wr_ptr(readsize);
		_chain.inbound_filter()->on_read( msg );
	}
}

void pipeline::handle_writeable( void ) {
	if ( _send_remains() ) {
		if ( !_send_queue.empty() )  {
			_io_context.evt(EPOLLIN|EPOLLOUT);
		} else {
			_io_context.evt(EPOLLIN);
		}
		if ( _loop.io_impl().register_handle(
					_socket.handle()
					, &_io_context ))
		{
			return;
		} else {
			return error_propagation(tdk::platform::error());
		}
	}
}

void pipeline::_error_propagation(const std::error_code& err ) {
	_socket.close();
	_chain.inbound_filter()->on_error(err);
}

void pipeline::error_propagation( const std::error_code& err  ) {
	if ( _state.fetch_or( detail::ERROR_BIT ) & detail::ERROR_BIT )
		return;
	// process next turn
	_loop.execute(task::make_one_shot_task(
					[this,err]{
						_error_propagation(err);
					}));
}

void pipeline::_on_closed( void ) {
	_socket.close();
	if ( (_state.fetch_or( detail::ERROR_BIT ) & detail::ERROR_BIT) == 0 )
		_chain.inbound_filter()->on_error( tdk::tdk_user_abort );

	_chain.inbound_filter()->on_closed();
	_loop.remove_active();
	delete this;
}

void pipeline::close( void ) {
	if ( _state.fetch_or( detail::CLOSE_BIT) & detail::CLOSE_BIT )
		return;
	// process next turn
	_loop.execute(task::make_one_shot_task(
					[this]{
						_on_closed();
					}));
}

void pipeline::write( tcp::message& msg ) {
	if ( _loop.in_loop() ) {
		_do_write(msg);
	} else {
		_loop.execute(task::make_one_shot_task(
							[this,msg]{
								tcp::message ref( msg );
								_do_write(ref);
							}));
	}
}

void pipeline::on_write( tcp::message& msg ) {
	bool process = _send_queue.empty();
	_send_queue.push_back( msg );
	if ( process ) {
		if ( _send_remains() ) {
			if ( !_send_queue.empty() )  {
				_io_context.evt(EPOLLIN|EPOLLOUT);
				if ( _loop.io_impl().register_handle(
							_socket.handle()
							, &_io_context ))
				{
					return;
				} else {
					return error_propagation(tdk::platform::error());
				}
			}
		}
	}
}

bool pipeline::_send_remains( void ) {
	tdk::io::buffer_adapter buf;
	while ( !_send_queue.empty()) {
		auto it = _send_queue.begin();
		while ( it != _send_queue.end()) {
			if ( it->data().length() == 0 ) {
				_send_queue.erase( it++ );
			} else {
				if ( !buf.push_back( it->data().rd_ptr() , it->data().length())){
					break;
				}
				++it;
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
				return true;
			} else {
				error_propagation(tdk::platform::error());
				return false;
			}
		} else {
			while ( writesize > 0 ) {
				auto msg = _send_queue.begin();
				int move = msg->data().rd_ptr(writesize);
				if (msg->data().length() == 0 )
					_send_queue.pop_front();
				writesize -= move;
			}
		}
	}
	return true;
}

void pipeline::_do_write( tcp::message& msg ) {
	_chain.outbound_filter()->on_write(msg);
}

}}}}
