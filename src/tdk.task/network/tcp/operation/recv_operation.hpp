#ifndef __tdk_network_tcp_recv_operation_h__
#define __tdk_network_tcp_recv_operation_h__

#include <tdk.task/task/operation.hpp>
#include <tdk/buffer/memory_block.hpp>

namespace tdk {
namespace network {
namespace tcp {
class channel;
class recv_operation : public tdk::task::operation {
public:
	recv_operation( tdk::network::tcp::channel* c 
		, tdk::buffer::memory_block& mb  );
	virtual ~recv_operation( void );

	virtual void operator()( void );

	tdk::network::tcp::channel* channel( void );
	tdk::buffer::memory_block& buffer( void );
private:
	tdk::network::tcp::channel* _channel;
	tdk::buffer::memory_block _buffer;
};


}}}


#endif

