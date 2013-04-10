#include "stdafx.h"
#include <tdk.task/network/tcp/channel.hpp>

namespace tdk {
namespace network {
namespace tcp {

channel::channel( tdk::task::event_loop& loop )
	: event_loop_object( loop )
{
}

}}}