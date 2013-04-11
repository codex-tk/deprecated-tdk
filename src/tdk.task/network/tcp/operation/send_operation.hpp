#ifndef __tdk_network_tcp_send_operation_h__
#define __tdk_network_tcp_send_operation_h__

#include <tdk.task/task/operation.hpp>
#include <tdk/buffer/memory_block.hpp>

#include <vector>

namespace tdk {
namespace network {
namespace tcp {
class channel;
class send_operation : public tdk::task::operation {
public:
	send_operation( tdk::network::tcp::channel* c 
		, std::vector< tdk::buffer::memory_block >& buffers );
	virtual ~send_operation( void );

	virtual void operator()( void );

	tdk::network::tcp::channel* channel( void );
	std::vector< tdk::buffer::memory_block >& buffers( void );
	int total_req_size( void );
private:
	tdk::network::tcp::channel* _channel;
	std::vector< tdk::buffer::memory_block > _buffers;
	int _total_req_size;
};


}}}


#endif

