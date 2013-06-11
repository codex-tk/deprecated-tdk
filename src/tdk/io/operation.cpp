#include "stdafx.h"
#include <tdk/io/operation.hpp>

namespace tdk {
namespace io {

operation::operation( callback cb )
	: _callback ( cb ){

}

operation::~operation( void ) {

}

void operation::operator()( const tdk::error_code& e , int io_bytes ) {
	Internal = e.value();
	InternalHigh = reinterpret_cast< ULONG_PTR >(&e.category());
	Offset = io_bytes;
	_callback(this);
}

}}
