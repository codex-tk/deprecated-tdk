#include "stdafx.h"
#include <tdk.task/network/tcp/service/stream.hpp>
#include <tdk.task/task/event_loop.hpp>
#include <tdk/error/tdk_error_category.hpp>
#include <tdk/error/platform_error.hpp>
namespace tdk {
namespace network {
namespace tcp {
namespace detail {

class close_operation : public tdk::task::operation{
public:
	close_operation( stream* s ) 
		: _stream(s) {
	}
	virtual ~close_operation( void ) {

	}
	virtual void operator()(void ){
		_stream->handler()->on_close( _stream , _code );
		_stream->reset();
	}
	void set_error_code( const tdk::error_code& code ) {
		_code = code;
	}
private:
	stream* _stream;
	tdk::error_code _code;
};

class send_req_operation : public tdk::task::operation{
public:
	send_req_operation( stream* s , const tdk::buffer::memory_block& mb ) 
		: _stream(s) , _buffer(mb)
	{
		_stream->add_ref();
	}
	virtual ~send_req_operation(void){
	}
	virtual void operator()(void){
		_stream->dec_ref();
		_stream->send( _buffer );
		delete this;
	}
private:
	stream* _stream;
	tdk::buffer::memory_block _buffer;
};
}

stream::stream ( task::event_loop& loop )
	: _channel(loop)
	, _closed(false)
	, _sending(false)
	, _recv_op(nullptr)
	, _send_op(nullptr)
	, _close_op(nullptr)
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

	_close_op = new detail::close_operation( this );
	return true;
}

void stream::close() {
	close( tdk::tdk_error( tdk::errc::tdk_network_user_abort ));
}

void stream::close( const tdk::error_code& err ) {
	if ( task::event_loop::current() == &_channel.loop()){
		if ( _closed ) {
			return;
		} else {
			_closed = true;
			static_cast< detail::close_operation* >(_close_op)->set_error_code(err);
			_channel.close();
		}
		if ( _ref_count.compare_and_swap( 0 , 0 ) == 0 ) {
			_channel.loop().post( _close_op );
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

void stream::add_ref(void){
	_ref_count.increment();
}
void stream::dec_ref(void){
	_ref_count.decrement();
}

void stream::send( const tdk::buffer::memory_block& mb ) {
	if ( task::event_loop::current() == &_channel.loop()){
		if ( _closed )
			return;

		if ( _sending ) {
			_send_buffer.push_back( mb );
		}else{
			_ref_count.increment();
			_sending = true;
			_send_op->buffer( mb );
			_channel.send( _send_op );
		}
	} else {
		tdk::task::operation* req = new detail::send_req_operation( this , mb );
		_channel.loop().post(req);
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
	if ( _close_op ) 
		delete _close_op;
	_recv_op = nullptr;
	_send_op = nullptr;
	_close_op= nullptr;

	_send_buffer.clear();
	_sending = false;
	_closed = false;
}

void stream::_on_send( tdk::network::tcp::send_operation& r ) {
	_ref_count.decrement();
	if ( r.error() ){
		close(r.error());
		return;
	}
	int send = r.io_byte();
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
	handler()->on_send( this , send , remain );

	if ( _closed ) {
		return;
	}

	if ( remain_buffers.empty() ) {
		_sending = false;
	}else{
		_send_op->buffers( remain_buffers );
		_channel.send( _send_op );
	}
}

}}}