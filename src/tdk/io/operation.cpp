#include "stdafx.h"
#include <tdk/io/operation.hpp>

namespace tdk {
namespace io {

operation::operation( callback cb )
	: _callback ( cb ){

}

operation::~operation( void ) {

}

tdk::error_code operation::error( void ) {
	return tdk::error_code( Internal , *(error_category*)InternalHigh);
}

void operation::operator()( const tdk::error_code& e , int io_bytes ) {
	error( e );
	Offset = io_bytes;
	_callback(this);
}

int operation::io_bytes( void ) {
	return Offset;
}

void operation::reset( void ) {
	Internal = 0;
	InternalHigh  = 0;
	Offset = 0;
	OffsetHigh = 0;
	hEvent = 0;
}

void operation::error( const tdk::error_code& ec ) {
	Internal = ec.value();
	InternalHigh = reinterpret_cast< ULONG_PTR >(&ec.category());
}

}}
