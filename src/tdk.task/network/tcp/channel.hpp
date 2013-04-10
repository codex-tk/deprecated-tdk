#ifndef __tdk_network_tcp_channel_h__
#define __tdk_network_tcp_channel_h__

#include <tdk.task/task/event_loop_object.hpp>

namespace tdk {
namespace network {
namespace tcp {

class channel : public task::event_loop_object{
public:
	channel( tdk::task::event_loop& loop );
};

}}}


#endif