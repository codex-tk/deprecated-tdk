#include "stdafx.h"
#include <tdk.task/task/operation.hpp>

namespace tdk {
namespace task {

operation::operation( void ) {

}

operation::~operation( void ) {

}

std::error_code operation::error( void ) {	
	return std::error_code( Internal
				, *reinterpret_cast< std::error_category* >( InternalHigh ));
}

void operation::error( const std::error_code& code ) {
	Internal = code.value();
	InternalHigh = reinterpret_cast< ULONG_PTR >(&code.category());
}

int  operation::io_byte( void ) {
	return this->Offset;
}

void operation::io_byte( int b) {
	this->Offset = b;
}

void* operation::object( void ) {
	return this->hEvent;
}

void  operation::object( void* o ) {
	this->hEvent = 0;
}

void operation::reset( void ) {
	Internal = 0;
	InternalHigh  = 0;
	Pointer = 0;
	hEvent = 0;
}

}}