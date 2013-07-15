#include "stdafx.h"
#include <tdk/io/operation.hpp>

namespace tdk {
namespace io {

operation::operation( callback cb )
	: _callback ( cb ){

}

operation::~operation( void ) {

}

std::error_code operation::error( void ) {
#if defined( _WIN32 )
    return std::error_code( static_cast<int>(Internal) , *(std::error_category*)InternalHigh);
#else
    return _error;
#endif
}

void operation::operator()( const std::error_code& e , int io_bytes ) {
	error( e );
#if defined( _WIN32 )    
	Offset = io_bytes;
#else
    _io_bytes = io_bytes;
#endif    
	_callback(this);
}

int operation::io_bytes( void ) {
#if defined( _WIN32 )    
	return Offset;
#else
    return _io_bytes;
#endif    
}

void operation::reset( void ) {
#if defined( _WIN32 )    
	Internal = 0;
	InternalHigh  = 0;
	Offset = 0;
	OffsetHigh = 0;
	hEvent = 0;
#endif    
}

void operation::error( const std::error_code& ec ) {
#if defined( _WIN32 )    
	Internal = ec.value();
	InternalHigh = reinterpret_cast< ULONG_PTR >(&ec.category());
#else
    _error = ec;
#endif    
}

}}
