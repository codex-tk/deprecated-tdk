#include <tdk/io/ip/tcp/operation/recv_operation_epoll.hpp>
#include <tdk/io/ip/tcp/socket_epoll.hpp>
#include <tdk/error/error_platform.hpp>
#include <tdk/error/error_tdk.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

recv_operation::recv_operation( tdk::io::operation::callback cb 
		, tdk::io::ip::tcp::socket& fd
		, const tdk::io::buffer_adapter& buffer  )
    : tdk::io::operation( cb )
    , _socket( &fd )
    , _buffer_adapter( buffer )
{
    
}

recv_operation::~recv_operation( void ) {

}

bool recv_operation::end_operation( void ) {
	if ( !error() && io_bytes() == 0 ) {
		int req_size = _buffer_adapter.size();
		if ( req_size != 0 ) {
			error( tdk::tdk_network_remote_closed);
		}
	}
    return true;
}

tdk::io::ip::tcp::socket& recv_operation::socket( void ) {
    return *_socket;
}

const tdk::io::buffer_adapter& recv_operation::buffer_adapter( void ) {
    return _buffer_adapter;
}

}}}}
