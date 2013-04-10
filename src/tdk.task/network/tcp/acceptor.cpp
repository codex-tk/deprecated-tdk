#include "stdafx.h"
#include <tdk.task/network/tcp/acceptor.hpp>

namespace tdk {
namespace network {
namespace tcp {

acceptor::acceptor( tdk::task::event_loop& loop )
	: event_loop_object( loop ){
}

}}}
