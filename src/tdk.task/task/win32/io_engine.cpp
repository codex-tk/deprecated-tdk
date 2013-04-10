#include "stdafx.h"
#include <tdk.task/task/operation.hpp>
#include <tdk.task/task/win32/io_engine.hpp>
#include <tdk.task/task/win32/io_contexts.hpp>
#include <tdk.task/task/event_loop.hpp>
namespace tdk{
namespace task {
namespace {
static int POSTED_OPERATION = 0xffffffff;
}

io_engine::io_engine( event_loop& loop ) 
	: _loop( loop )
	, _port( *this )
{

}

io_engine::~io_engine( void ) {

}


void io_engine::on_complete( const tdk::error_code& code 
		, int io_byte 
		, void* key
		, OVERLAPPED* ov )
{
	if ( io_byte == POSTED_OPERATION ) {
		tdk::task::operation* op = static_cast< tdk::task::operation* >( key );
		if ( op ) {
			(*op)();
		}
		return;
	}
	if ( ov == nullptr ){
		return;
	}
	IO_CONTEXT* io_ctx = static_cast< IO_CONTEXT* >( ov );
	if ( io_ctx->event == IO_EVENT::RECV ) {
		RECV_IO_CONTEXT* ctx = static_cast< RECV_IO_CONTEXT* >( io_ctx ); 
		_loop.on_recv_complete();
	} else if ( io_ctx->event == IO_EVENT::SEND ) {
		SEND_IO_CONTEXT* ctx = static_cast< SEND_IO_CONTEXT* >( io_ctx ); 
		_loop.on_send_complete();
	} else if ( io_ctx->event == IO_EVENT::ACCEPT ) {
		ACCEPT_IO_CONTEXT* ctx = static_cast< ACCEPT_IO_CONTEXT* >( io_ctx ); 
		_loop.on_accept_complete();
	} else if ( io_ctx->event == IO_EVENT::CONNECT ) {
		CONNECT_IO_CONTEXT* ctx = static_cast< CONNECT_IO_CONTEXT* >( io_ctx ); 
		_loop.on_connect_complete();
	}
}

}}