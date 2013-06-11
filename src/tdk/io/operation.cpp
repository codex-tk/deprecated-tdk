#include "stdafx.h"
#include <tdk/io/operation.hpp>

namespace tdk {
namespace io {

operation::operation( callback cb )
	: _callback ( cb ){

}

operation::~operation( void ) {

}

void operation::operator()( void ) {
	_callback(this);
}

}}
