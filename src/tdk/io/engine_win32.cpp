#include "stdafx.h"
#include <tdk/io/engine_win32.hpp>
#include <tdk/io/operation.hpp>

namespace tdk {
namespace io {
namespace detail {

static void on_port_callback ( 
	const tdk::error_code& code 
		, int io_byte 
		, void* 
		, OVERLAPPED* ov
		, void* ctx )
{
	operation* op( static_cast< operation* >(ov));
	(*op)( code , io_byte );
}

}

engine::engine( void ) {

}

engine::~engine( void ) {

}

bool engine::open( void ) {
	return _port.create();
}

void engine::close( void ) {
	_port.close();
}

bool engine::run( const tdk::time_span& wait ) {
	return _port.wait( wait , &detail::on_port_callback , this ) >= 0;
}

}}
