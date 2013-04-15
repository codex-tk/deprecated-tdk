#ifndef __tdk_network_tcp_accept_operation_h__
#define __tdk_network_tcp_accept_operation_h__

#include <tdk.task/task/operation.hpp>
#include <tdk/network/socket.hpp>

namespace tdk {
namespace network {
namespace tcp {
class acceptor;
class accept_operation : public tdk::task::operation {
public:
	explicit accept_operation( tdk::network::tcp::acceptor& a );
	virtual ~accept_operation( void );

	tdk::network::socket& socket( void );
	sockaddr_storage* address_ptr( void );
	tdk::network::tcp::acceptor& acceptor( void );
private:
	tdk::network::socket _fd;
	tdk::network::tcp::acceptor* _acceptor;
	sockaddr_storage _address[2];
};


}}}


#endif