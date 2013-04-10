#ifndef __tdk_network_tcp_acceptor_h__
#define __tdk_network_tcp_acceptor_h__

#include <tdk.task/task/event_loop_object.hpp>
#include <tdk/network/socket.hpp>

namespace tdk {
namespace network {
namespace tcp {

class acceptor: public task::event_loop_object{
public:
	acceptor( tdk::task::event_loop& loop );

	bool open( const tdk::network::address& addr );
	void close( void );

	void accept( void );
	tdk::network::socket& socket( void );
	const tdk::network::address& address( void );
private:
	tdk::network::socket _fd;
	tdk::network::address _address;
};

}}}


#endif