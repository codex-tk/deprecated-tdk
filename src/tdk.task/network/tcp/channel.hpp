#ifndef __tdk_network_tcp_channel_h__
#define __tdk_network_tcp_channel_h__

#include <tdk.task/task/event_loop_object.hpp>
#include <tdk/network/socket.hpp>
#include <tdk/buffer/memory_block.hpp>

namespace tdk {
namespace network {
namespace tcp {

class channel : public task::event_loop_object{
public:
	channel( tdk::task::event_loop& loop );
	
	bool open( tdk::network::socket& fd );
	void close( void );

	void recv( tdk::buffer::memory_block& mb );

	tdk::network::socket& socket( void );
private:
	tdk::network::socket _fd;
};

}}}


#endif