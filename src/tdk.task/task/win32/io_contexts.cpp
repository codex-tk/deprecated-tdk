#include "stdafx.h"
#include <tdk.task/task/win32/io_contexts.hpp>

namespace tdk{
namespace task {

void io_context::reset( void ) {
	Internal = 0;
	InternalHigh  = 0;
	Pointer = 0;
	hEvent = 0;
}

tdk::error_code io_context::error( void ) {
	return tdk::error_code( Internal
				, *reinterpret_cast< tdk::error_category* >( InternalHigh ));
}

void io_context::error( const tdk::error_code& code ) {
	Internal = code.value();
	InternalHigh = reinterpret_cast< ULONG_PTR >(&code.category());
}
		
accept_io_context::accept_io_context( void ){
	event = io_event::accept;
}

connect_io_context::connect_io_context( void ){
	event = io_event::connect;
}

recv_io_context::recv_io_context( void ){
	event = io_event::recv;
}

send_io_context::send_io_context( void ){
	event = io_event::send;
}

}}