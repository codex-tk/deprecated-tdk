#include "stdafx.h"
#include <tdk.task/network/tcp/service/service.hpp>

namespace tdk{
namespace network{
namespace tcp{

service::service( tdk::task::event_loop& loop )
	: event_loop_object(loop)
	, _acceptor( loop )
{
}
service::~service( void ){
}


}}}