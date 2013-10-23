#include "stdafx.h"
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
	, _on_recv( &channel::_handle_recv , this )
	, _on_send( &channel::_handle_send , this )
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
	_loop.execute(task::make_one_shot_task(
					[this]{
						fire_on_close();
						release();
					}));
}

void channel::write( tdk::buffer::memory_block& msg ){
	retain();
	_loop.execute(task::make_one_shot_task(
						[this,msg]{
							fire_do_write(msg);
							release();
						}));
}

void channel::fire_on_connected(void) {
	retain();
	_pipeline.in_bound_filter()->on_connected();
	_do_recv();
}


void channel::fire_on_accepted( const tdk::io::ip::address& addr ) {
	retain();
	_pipeline.in_bound_filter()->on_accepted(addr);
	_do_recv();
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
	_loop.execute(task::make_one_shot_task(
					[this,err]{
						fire_on_error(err);
						release();
					}));
}

tcp::pipeline& channel::pipeline( void ) {
	return _pipeline;
}

void channel::_do_recv( void ) {
	if ( _state.load() != 0 )
		return;

	_recv_buffer = tdk::buffer::memory_block( channel_config().recv_buffer_size );
	DWORD flag = 0;
    WSABUF buffer;
	buffer.buf = reinterpret_cast< CHAR* >(_recv_buffer.wr_ptr());
    buffer.len = channel_config().recv_buffer_size;
	_on_recv.reset();
	if ( WSARecv(	_socket.handle() 
                    , &buffer
                    , 1
                    , nullptr 
                    , &flag 
					, _on_recv.impl()
                    , nullptr ) == SOCKET_ERROR )
    {
        std::error_code ec = tdk::platform::error();
        if ( ec.value() != WSA_IO_PENDING ){
			_error_propagation( ec );
			return;
        }
    }
	retain();
	_loop.add_active();
}

void channel::do_write(tdk::buffer::memory_block& msg){
	bool process = _send_queue.empty();
	_send_queue.push_back(msg);
	if (!process)
		return;

	_send_remains();
}

void channel::handle_recv( void ) {	
	if ( _state.load() != 0 )
		return;

	if ( _on_recv.io_bytes() == 0 ) {
		_on_recv.error( tdk::tdk_network_remote_closed );
	}
	if ( _on_recv.error() ) {
		_error_propagation( _on_recv.error());
		return;
	} 
	_recv_buffer.wr_ptr( _on_recv.io_bytes());
	fire_on_read(_recv_buffer);
	_do_recv();
}

void channel::handle_send( void ) {
	if ( _state.load() != 0 )
		return;

	if ( _on_send.error() ) {
		_error_propagation( _on_send.error());
		return;
	}

	int total_write = _on_send.io_bytes();
	while ( total_write > 0 ) {
		auto msg = _send_queue.begin();
		int move = msg->rd_ptr(total_write);
		if (msg->length() == 0 )
			_send_queue.pop_front();
		total_write -= move;
	}
	bool flushed = _send_queue.empty();
	fire_on_write(total_write , flushed);
	if ( !flushed )
		_send_remains();
}


void channel::_send_remains( void ) {
	if ( _state.load() != 0 )
		return;
	tdk::io::buffer_adapter buf;
	for( auto it : _send_queue ) {
		if ( !buf.push_back( it.rd_ptr() , it.length())){
			break;
		}
	}
	_on_send.reset();
    DWORD flag	= 0;
    if ( WSASend(	_socket.handle() 
					, const_cast< LPWSABUF >( buf.buffers())
					, buf.count()
                    , nullptr 
                    , flag 
					, _on_send.impl()
                    , nullptr ) == SOCKET_ERROR )
    {
        std::error_code ec = tdk::platform::error();
        if ( ec.value() != WSA_IO_PENDING ){
			_on_send.error( ec );
			_loop.execute(&_on_send);
        }
    }
	retain();
	_loop.add_active();
}

void channel::_handle_recv( tdk::task* t ) {
	channel* c = static_cast< channel* >( t->context());
	c->handle_recv();
	c->loop().remove_active();
	c->release();
}

void channel::_handle_send( tdk::task* t ) {
	channel* c = static_cast< channel* >( t->context());
	c->handle_send();
	c->loop().remove_active();
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

}}}}