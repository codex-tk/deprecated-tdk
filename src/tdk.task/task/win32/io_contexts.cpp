#include "stdafx.h"
#include <tdk.task/task/win32/io_contexts.hpp>

namespace tdk{
namespace task {
		
ACCEPT_IO_CONTEXT::ACCEPT_IO_CONTEXT( void ){
	event = IO_EVENT::ACCEPT;
}

CONNECT_IO_CONTEXT::CONNECT_IO_CONTEXT( void ){
	event = IO_EVENT::CONNECT;
}

RECV_IO_CONTEXT::RECV_IO_CONTEXT( void ){
	event = IO_EVENT::RECV;
}

SEND_IO_CONTEXT::SEND_IO_CONTEXT( void ){
	event = IO_EVENT::SEND;
}

}}