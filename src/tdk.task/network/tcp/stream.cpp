#include "stdafx.h"
#include <tdk.task/network/tcp/stream.hpp>
#include <tdk.task/task/event_loop.hpp>
#include <tdk/error/tdk_error_category.hpp>
#include <tdk/error/platform_error.hpp>
namespace tdk {
namespace network {
namespace tcp {

stream::stream ( task::event_loop& loop )
	: _channel(loop)
	, _closed(false)
	, _sending(false)
	, _recv_op(nullptr)
	, _send_op(nullptr)
	, _close_posted( false )
{
	
}

stream::~stream ( void ) {
	
}

// for accept
bool stream::open( tdk::network::socket& fd  , stream_handler* handler ){
	if ( _channel.open( fd ) ) {
		return open( handler );
	}
	return false;
}
// for connect
bool stream::open( stream_handler* handler ) {
	_handler = handler;
	_recv_op = _channel.create_recv_operation(
		[this]( tdk::network::tcp::recv_operation& r ) {
			_on_recv(r);
		});
	_send_op = _channel.create_send_operation( 
		[this]( tdk::network::tcp::send_operation& r ) {
			_on_send(r);
		});

	return true;
}

void stream::close() {
	close( tdk::tdk_error( tdk::errc::tdk_network_user_abort ));
}

void stream::close( const tdk::error_code& code ) {
	if ( task::event_loop::current() == &_channel.loop()){
		if (_close_posted)
			return;

		if ( !_closed ) {
			_closed = true;
			_code = code;
			_channel.close();			
		}

		if ( _ref_count.compare_and_swap( 0 , 0 ) == 0 ) {
			_close_posted = true;
			_channel.loop().post( [this](){
				_handler->on_close( this , _code );
				reset();
			});
		}
	} else {
		_ref_count.increment();
		_channel.loop().post([this]{
			_ref_count.decrement();
			close();
		});
	}
}

void stream::recv( const tdk::buffer::memory_block& mb ) {
	_ref_count.increment();
	_recv_op->buffer( mb );
	_channel.recv( _recv_op );
}

void stream::send( const tdk::buffer::memory_block& mb ) {
	if ( task::event_loop::current() == &_channel.loop()){
		if ( _closed )
			return;

		if ( _sending ) {
			_send_buffer.push_back( mb );
		} else {
			_ref_count.increment();
			_sending = true;
			_send_op->buffer( mb );
			_channel.send( _send_op );
		}
	} else {
		if ( _closed )
			return;
		_channel.loop().post([ this , mb ] (){
			send(mb);
		});
	}
}

void stream::_on_recv( tdk::network::tcp::recv_operation& r ) {
	_ref_count.decrement();
	if ( r.error() ) {
		close(r.error());
		return;
	}

	if ( r.io_byte() == 0 && r.buffer().space() != 0 ) {
		close(  tdk::tdk_error( tdk::errc::tdk_network_remote_closed ) );
		return;
	}
	handler()->on_recv( this , r.buffer() );
}

void stream::reset( void ) {
	if ( _recv_op ) 
		delete _recv_op;
	if ( _send_op ) 
		delete _send_op;
	_recv_op = nullptr;
	_send_op = nullptr;

	_send_buffer.clear();
	_sending = false;
	_closed = false;
	_close_posted = false;
}

void stream::_on_send( tdk::network::tcp::send_operation& r ) {
	_ref_count.decrement();
	if ( r.error() ){
		close(r.error());
		return;
	}
	int sent = r.io_byte();
	int remain = 0;
	std::vector< tdk::buffer::memory_block > remain_buffers;
	if ( r.total_req_size() != r.io_byte() ) {
		for ( auto it : r.buffers() ) {
			if ( it.length() > 0 ) {
				remain_buffers.push_back(it);
			}
		}
	}
	for( auto it : _send_buffer ) {
		remain_buffers.push_back( it );
	}
	_send_buffer.clear();

	if ( !remain_buffers.empty() ) {
		for ( auto it : remain_buffers ) {
			remain += it.length();
		}
	}
	if ( remain_buffers.empty() ) {
		_sending = false;
	} else {
		_send_op->buffers( remain_buffers );
		_channel.send( _send_op );
	}
	handler()->on_send( this , sent , remain );
}

}}}