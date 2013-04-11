#ifndef __tdk_network_tcp_connect_operation_h__
#define __tdk_network_tcp_connect_operation_h__

#include <tdk.task/task/operation.hpp>
#include <tdk/network/socket.hpp>

namespace tdk {
namespace network {
namespace tcp {
class channel;
class connect_operation : public tdk::task::operation {
public:
	connect_operation( tdk::network::tcp::channel* c 
		, const tdk::network::address& addr  );
	virtual ~connect_operation( void );
	
	tdk::network::tcp::channel* channel( void );
	tdk::network::address& address( void );
private:
	tdk::network::tcp::channel* _channel;
	tdk::network::address _address;
};
}}}


#endif

