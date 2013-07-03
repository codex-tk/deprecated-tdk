#include "stdafx.h"
#include <tdk.task/network/tcp/stream.hpp>
#include <tdk.task/task/event_loop.hpp>
#include <tdk/error/error_category_tdk.hpp>
#include <tdk/error/error_platform.hpp>
namespace tdk {
namespace network {
namespace tcp {

stream::stream ( task::event_loop& loop )
	: _channel(loop)
	, _closed(false)
	, _sending(false)
	, _recv_op(nullptr)
	, _send_op(nullptr)
	, _error_post_status(post_status::not_post)
	, _close_post_status(post_status::not_post)
{
	
}

stream::~stream ( void ) {
	reset();
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
	channel().loop().increment_ref();
	return true;
}

bool stream::_post_error( const std::error_code& code ) {
	if ( _error_post_status == post_status::not_post ) {
		_error_post_status  = post_status::begin_post;
		_channel.loop().post( [this , code](){
			do {
				tdk::threading::scoped_lock<> gaurd(_lock);
				_error_post_status  = post_status::end_post;
				_post_close();
			}while(0);
			_handler->on_error( *this , code );	
			
		});
		return true;
	}
	return false;
}

void stream::_post_close( void ) {
	if ( !_closed )
		return;	

	if ( _close_post_status != post_status::not_post ) 
		return;

	if ( _error_post_status != post_status::end_post ) 
		return;

	int exp = 0;
	int change = 0;
	if ( _ref_count.compare_exchange_strong( exp , change )){
		_close_post_status = post_status::begin_post;
		_channel.loop().post( [this](){
			_close_post_status = post_status::end_post;
			channel().loop().decrement_ref();
			_handler->on_close( *this );
		});
	}
}

void stream::close() {
	_internal_close(tdk::error( tdk::errc::tdk_network_user_abort ) , true );
}

void stream::recv( const tdk::buffer::memory_block& mb ) {
	++_ref_count;
	_recv_op->buffer( mb );
	_channel.recv( _recv_op );
}

void stream::send( const tdk::buffer::memory_block& mb ) {
	tdk::threading::scoped_lock<> gaurd(_lock);
	if ( _is_closed() )
		return;
	if ( _sending ) {
		_send_buffer.push_back( mb );
	} else {
		++_ref_count;
		_sending = true;
		_send_op->buffer( mb );
		_channel.send( _send_op );
	}
}

void stream::_internal_close( const std::error_code& code , bool closed  ) {
	tdk::threading::scoped_lock<> gaurd(_lock);
	if ( !_closed )
		_closed = closed;
	_channel.close();
	if ( _post_error( code ) ) 
		return;
	_post_close();
}

bool stream::_is_closed(void) {
	if ( _closed )
		return true;
	if ( _error_post_status != post_status::not_post ) 
		return true;
	return false;
}

void stream::_on_recv( tdk::network::tcp::recv_operation& r ) {
	--_ref_count;
	if ( r.error() ) {
		_internal_close( r.error());
		return;
	}
	if ( r.io_byte() == 0 && r.buffer().space() != 0 ) {
		_internal_close( tdk::error( tdk::errc::tdk_network_remote_closed ));
		return;
	}
	handler()->on_recv( *this , r.buffer() );
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
	_error_post_status = post_status::not_post;
	_close_post_status = post_status::not_post;
}

void stream::_on_send( tdk::network::tcp::send_operation& r ) {
	--_ref_count;
	if ( r.error() ){
		_internal_close( r.error());
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
	do {
		tdk::threading::scoped_lock<> gaurd(_lock);
		for( auto it : _send_buffer ) {
			remain_buffers.push_back( it );
		}
		_send_buffer.clear();
		if ( remain_buffers.empty() ) {
			_sending = false;
		}
	}while(0);

	if ( !remain_buffers.empty() ) {
		for ( auto it : remain_buffers ) {
			remain += it.length();
		}
		++_ref_count;
		_send_op->buffers( remain_buffers );
		_channel.send( _send_op );
	}
	handler()->on_send( *this , sent , remain );
}

stream_handler* stream::handler( void ) {
	return _handler;
}

tdk::network::tcp::channel& stream::channel(void) {
	return _channel;
}

void* stream::tag( void ) {
	return _tag;
}

void stream::tag( void* p ) {
	_tag = p;
}

}}}