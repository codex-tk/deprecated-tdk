#include "stdafx.h"
#include <tdk.task/network/tcp/service/stream.hpp>
#include <tdk.task/task/event_loop.hpp>
namespace tdk {
namespace network {
namespace tcp {

stream::stream ( task::event_loop& loop )
	: _channel( loop )
	, _io_ref(0)
{
	_recv_op = _channel.create_recv_operation( [this]( tdk::network::tcp::recv_operation& r ) {
		_on_recv(r);
	});
	_send_op = _channel.create_send_operation( [this]( tdk::network::tcp::send_operation& r ) {
		_on_send(r);
	});
}

stream::~stream ( void ) {
	delete _recv_op;
	delete _send_op;
}

// for accept
bool stream::open( tdk::network::socket& fd  , stream_handler* handler ){
	if ( _channel.open( fd ) ) {
		_handler = handler;
		return true;
	}
	return false;
}
// for connect
bool stream::open( stream_handler* handler ) {
	_handler = handler;
	return true;
}

void stream::close( void ) {
	if ( task::event_loop::current() == &_channel.loop()){
		if ( _closed ) {
			return;
		}
		_channel.close();
		if ( _io_ref == 0 ) {
			//onclose
		}
	} else {
		_channel.loop().post([this]{
			close();
		});
	}
}

void stream::recv( const tdk::buffer::memory_block& mb ) {
	_recv_op->buffer( mb );
	_channel.recv( _recv_op );
}

void stream::send( const tdk::buffer::memory_block& mb ) {
	if ( task::event_loop::current() == &_channel.loop()){
		
	} else {
		class send_req_operation : public tdk::task::operation{
		public:
			send_req_operation( stream* s , const tdk::buffer::memory_block& mb ) 
				: _stream(s) , _buffer(mb)
			{
			}
			virtual ~send_req_operation(void){
			}
			virtual void operator()(void){
				_stream->send( _buffer );
				delete this;
			}
		private:
			stream* _stream;
			tdk::buffer::memory_block _buffer;
		};
		tdk::task::operation* req = new send_req_operation( this , mb );
		_channel.loop().post(req);
	}
}

void stream::_on_recv( tdk::network::tcp::recv_operation& r ) {

}

void stream::_on_send( tdk::network::tcp::send_operation& r ) {

}

}}}