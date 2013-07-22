#include <tdk/io/ip/tcp/operation/accept_operation_epoll.hpp>
#include <tdk/io/ip/tcp/socket_epoll.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

accept_operation::accept_operation( tdk::io::operation::callback cb
		, tdk::io::ip::tcp::socket& listenfd
		, tdk::io::ip::tcp::socket& fd)
		: tdk::io::operation( cb )
		, _socket( &fd )
		, _listen_socket( &listenfd )
{

}

accept_operation::~accept_operation( void ) {

}

bool accept_operation::end_operation( void ) {
	return true;
}


tdk::io::ip::tcp::socket& accept_operation::socket( void ) {
	return *_socket;
}

tdk::io::ip::tcp::socket& accept_operation::listen_socket( void ) {
	return *_listen_socket;
}
	

}}}}
