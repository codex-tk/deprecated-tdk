#ifndef __tdk_network_tcp_acceptor_h__
#define __tdk_network_tcp_acceptor_h__

#include <tdk.task/task/event_loop_object.hpp>

namespace tdk {
namespace network {
namespace tcp {

class acceptor: public task::event_loop_object{
public:
	acceptor( tdk::task::event_loop& loop );
};

}}}


#endif