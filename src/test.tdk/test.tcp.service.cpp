#include "stdafx.h"
#include <tdk.task/task/event_loop.hpp>
#include <tdk.task/network/tcp/acceptor.hpp>
#include <tdk.task/network/tcp/connector.hpp>
#include <tdk.task/network/tcp/channel.hpp>
#include <tdk.task/network/tcp/service/service.hpp>
#include <tdk/log/logger.hpp>

TEST( event_loop , init ){
	tdk::task::event_loop loop;
	ASSERT_TRUE( loop.open());
	tdk::network::tcp::service svc( loop );


	

	loop.close();
}