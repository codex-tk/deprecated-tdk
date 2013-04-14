#ifndef __tdk_network_udp_recvfrom_operation_h__
#define __tdk_network_udp_recvfrom_operation_h__

#include <tdk.task/task/operation.hpp>
#include <tdk/buffer/memory_block.hpp>
#include <tdk/network/address.hpp>

namespace tdk {
namespace network {
namespace udp {
class channel;
class recvfrom_operation : public tdk::task::operation {
public:
	recvfrom_operation( tdk::network::udp::channel* c );
	recvfrom_operation( tdk::network::udp::channel* c
		, const tdk::buffer::memory_block& mb );

	virtual ~recvfrom_operation( void );

	tdk::network::udp::channel* channel( void );
	tdk::buffer::memory_block& buffer( void );
	tdk::network::address& address( void );
	void buffer( const tdk::buffer::memory_block& mb );

	void before_dispatch( void );
	void after_dispatch( void );
private:
	tdk::network::udp::channel* _channel;
	tdk::network::address _address;
	tdk::buffer::memory_block _buffer;
};


}}}


#endif

