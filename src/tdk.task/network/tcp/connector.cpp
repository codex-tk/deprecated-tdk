#include "stdafx.h"
#include <tdk.task/network/tcp/connector.hpp>
#include <tdk.task/task/event_loop.hpp>
namespace tdk {
namespace network {
namespace tcp {

void connector::connect( tdk::network::tcp::connect_operation* op ) {
	op->channel().loop().engine().add_connect_io( op );
}

}}}