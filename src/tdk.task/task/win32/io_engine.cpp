#include "stdafx.h"
#include <tdk.task/task/operation.hpp>
#include <tdk.task/task/win32/io_engine.hpp>
#include <tdk.task/task/win32/io_contexts.hpp>
#include <tdk.task/task/event_loop.hpp>
#include <tdk/error/platform_error.hpp>


namespace tdk{
namespace task {
namespace detail{
static int posted_operation = 0xffffffff;

static void on_complete( const tdk::error_code& code 
		, int io_byte 
		, void* key
		, OVERLAPPED* ov
		, void* ctx ) 
{
	tdk::task::io_engine* engine = static_cast<tdk::task::io_engine*>(ctx);
	engine->on_complete( code , io_byte , key , ov );
}

}

io_engine::io_engine( event_loop& loop ) 
	: _loop( loop )
{

}

io_engine::~io_engine( void ) {

}

bool io_engine::open( void ) {
	return _port.open( &detail::on_complete , this );
}

void io_engine::close( void ){
	_port.close();
}

bool io_engine::bind( SOCKET fd , void* obj ) {
	return _port.bind( fd , obj );
}


void io_engine::add_accept_io( tdk::network::tcp::acceptor& acceptor ) {
	accept_io_context* ctx = new accept_io_context();
	ctx->reset();
	ctx->acceptor = &acceptor;
	if ( !ctx->fd.open_tcp( acceptor.address().family() )) {
		ctx->error( platform_error() );
		_post( ctx );
		return;
	}
	DWORD dwBytes = 0;
	if ( AcceptEx(  acceptor.socket().handle() ,
					ctx->fd.handle() ,
					ctx->address ,
                    0 ,
					sizeof( sockaddr_storage ) ,
                    sizeof( sockaddr_storage ) ,
                    &dwBytes , 
                    static_cast< OVERLAPPED*>(ctx) ) == FALSE ) 
    {
		tdk::error_code ec = tdk::platform_error();
		if ( ec.value() != WSA_IO_PENDING ){
			ctx->error( ec );
			_post( ctx );
		}
    }
}

void io_engine::add_recv_io( 
		tdk::network::tcp::channel& chan
		, tdk::buffer::memory_block& mb )
{
	recv_io_context* ctx = new recv_io_context();
	ctx->reset();
	ctx->channel = &chan;
	ctx->recv_buffer = mb;
	DWORD flag	= 0;
	WSABUF buffer;
	buffer.buf = (CHAR*)mb.wr_ptr();
	buffer.len = mb.space();
	if ( WSARecv(	chan.socket().handle() 
					, &buffer
					, 1
					, nullptr 
					, &flag 
					, ctx
					, nullptr ) == SOCKET_ERROR )
	{
		tdk::error_code ec = tdk::platform_error();
		if ( ec.value() != WSA_IO_PENDING ){
			ctx->error( ec );
			_post( ctx );
		}
	}
}

void io_engine::_post( io_context* ctx ) {
	if ( !_port.post( 0 , nullptr , ctx )) {
		tdk::threading::scoped_lock<> gaurd( _post_fail_lock );
		_post_fails.push_back( ctx );
		_post_failed.exchange(1);
	}
}

bool io_engine::run( const tdk::time_span& wait ) {
	if( _post_failed.compare_and_swap( 0 , 1 ) == 1 ) {
		std::vector< io_context* > ctxs;
		do {
			tdk::threading::scoped_lock< > guard( _post_fail_lock );
			ctxs = _post_fails;
			_post_fails.clear();
		}while(0);
		for ( auto ctx : ctxs ) {
			on_complete( ctx->error() , 0 , ctx->Pointer , ctx );
		}
	}
	return _port.wait( wait ) >= 0;
}

void io_engine::on_complete( const tdk::error_code& code 
		, int io_byte 
		, void* key
		, OVERLAPPED* ov )
{
	tdk::error_code real_err = code;
	io_context* io_ctx = static_cast< io_context* >( ov );
	if ( io_byte == detail::posted_operation ){
		if ( io_ctx == nullptr ) {
			tdk::task::operation* op = static_cast< tdk::task::operation* >( key );
			if ( op ) {
				(*op)();
			}
			return;
		} else {
			real_err = io_ctx->error();
			io_byte = 0;
		}
	}
	
	if ( io_ctx->event == io_event::recv ) {
		recv_io_context* ctx = static_cast< recv_io_context* >( io_ctx ); 
		ctx->recv_buffer.wr_ptr( io_byte );
		_loop.on_recv_complete( real_err
			, *ctx->channel
			, ctx->recv_buffer );
		delete ctx;
	} else if ( io_ctx->event == io_event::send ) {
		send_io_context* ctx = static_cast< send_io_context* >( io_ctx ); 
		_loop.on_send_complete();
	} else if ( io_ctx->event == io_event::accept ) {
		accept_io_context* ctx = static_cast< accept_io_context* >( io_ctx ); 
		_loop.on_accept_complete(
			real_err
			, *ctx->acceptor
			, ctx->fd );
		delete ctx;
	} else if ( io_ctx->event == io_event::connect ) {
		connect_io_context* ctx = static_cast< connect_io_context* >( io_ctx ); 
		_loop.on_connect_complete();
	}
	
}

}}