#include "stdafx.h"
#include <tdk.task/network/tcp/operation/connect_operation.hpp>


namespace tdk {
namespace network {
namespace tcp {

connect_operation::connect_operation( tdk::network::tcp::channel& c 
	, const tdk::network::address& addr )
	: _channel(&c)
	, _address( addr )
{
}

connect_operation::~connect_operation( void ) {
}

tdk::network::tcp::channel& connect_operation::channel( void ) {
	return *_channel;
}

tdk::network::address& connect_operation::address( void ) {
	return _address;
}

}}}