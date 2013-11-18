#include "stdafx.h"
#include <tdk/io/ip/udp/channel.hpp>
#include <tdk/io/ip/udp/pipeline/filter.hpp>
#include <tdk/io/buffer_adapter.hpp>
#include <tdk/event_loop/event_loop.hpp>
#include <tdk/error/error_platform.hpp>
#include <tdk/error/error_tdk.hpp>

#if defined( _WIN32 )
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
	, _on_recv( &channel::_handle_recv , this )
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
							_do_recv();
							release();
						}));
	}
}

bool channel::is_bits_on( int b ) {
	return ( _state.load() & b ) != 0;
}

void channel::fire_on_open(void) {
	retain();
	_pipeline.in_bound_filter()->on_open();
	_do_recv();
}

void channel::fire_on_read( const tdk::io::ip::address& addr , tdk::buffer::memory_block& msg  ) {
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
	release();
}

void channel::fire_do_write( const tdk::io::ip::address& addr 
							, tdk::buffer::memory_block msg ) {
	if ( is_bits_on( detail::k_error_bit | detail::k_close_bit ))
		return;
	_pipeline.out_bound_filter()->do_write(addr,msg);
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

udp::pipeline& channel::pipeline( void ) {
	return _pipeline;
}

tdk::io::ip::socket& channel::socket_impl( void ) {
	return _socket;
}

void channel::_do_recv( void ) {
	if ( is_bits_on( detail::k_error_bit 
					| detail::k_close_bit
					| detail::k_read_pending_bit ))
		return;

	int mtu = 1440;
	_recv_buffer = tdk::buffer::memory_block( mtu );
	DWORD flag = 0;
    WSABUF buffer;
	buffer.buf = reinterpret_cast< CHAR* >(_recv_buffer.wr_ptr());
    buffer.len = mtu;
	_on_recv.reset();
	socklen_t* len = _recv_addr.sockaddr_length_ptr();
	*len = sizeof( sockaddr_storage );
	if ( WSARecvFrom(_socket.handle() 
                    , &buffer
                    , 1
                    , nullptr 
                    , &flag 
					, _recv_addr.sockaddr()
					, len
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

void channel::do_write(const tdk::io::ip::address& addr
		, tdk::buffer::memory_block& msg)
{
	if ( is_bits_on( detail::k_error_bit | detail::k_close_bit))
		return;

	tdk::io::buffer_adapter buf;
	buf.push_back( msg.rd_ptr() , msg.length());
	DWORD sendbytes = 0;
	if ( WSASendTo( _socket.handle()
		, const_cast< LPWSABUF >( buf.buffers())
		, buf.count()
		, &sendbytes
		, 0
		, addr.sockaddr()
		, addr.sockaddr_length()
		, nullptr 
		, nullptr ) == SOCKET_ERROR)
	{
		error_propagation(tdk::platform::error());
		return;
	}
}

void channel::handle_recv( void ) {	
	if ( is_bits_on( detail::k_error_bit | detail::k_close_bit))
		return;

	if ( _on_recv.io_bytes() == 0 ) {
		_on_recv.error( tdk::tdk_network_remote_closed );
	}
	if ( _on_recv.error() ) {
		_error_propagation( _on_recv.error());
		return;
	} 
	_recv_buffer.wr_ptr( _on_recv.io_bytes());
	fire_on_read( _recv_addr , _recv_buffer);
	_do_recv();
}

void channel::_handle_recv( tdk::task* t ) {
	channel* c = static_cast< channel* >( t->context());
	c->handle_recv();
	c->loop().remove_active();
	c->release();
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

}}}}

#endif
