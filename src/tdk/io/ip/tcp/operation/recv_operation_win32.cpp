#include "stdafx.h"
#include <tdk/io/ip/tcp/operation/recv_operation_win32.hpp>
#include <tdk/io/ip/tcp/socket_win32.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

recv_operation::recv_operation( tdk::io::ip::tcp::socket& fd
    , const tdk::io::buffer_adapter& buffer
    , tdk::io::operation::callback cb )
    : tdk::io::operation( cb )
    , _socket( &fd )
    , _buffer_adapter( buffer )
{
    
}

recv_operation::~recv_operation( void ) {

}

bool recv_operation::end_opearation( void ) {
	if ( !error() && io_bytes() == 0 ) {
		int req_size = _buffer_adapter.size();
		if ( req_size != 0 ) {
			error( tdk::platform::error(WSAECONNRESET));
		}
	}
    return true;
}

void recv_operation::process( void ) {
    _socket->engine().async_recv( this );
}

tdk::io::ip::tcp::socket& recv_operation::socket( void ) {
    return *_socket;
}

const tdk::io::buffer_adapter& recv_operation::buffer_adapter( void ) {
    return _buffer_adapter;
}

}}}}