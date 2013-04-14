#include "stdafx.h"

#include <tdk/network/socket.hpp>

TEST( accept , t1 )
{
	tdk::network::socket accept_fd;

	accept_fd.open_tcp( AF_INET );
	accept_fd.bind( tdk::network::address::any(10001));
	accept_fd.listen();

	accept_fd.wait_for_recv( tdk::time_span::infinite());
	tdk::network::address addr;
	tdk::network::socket fd = accept_fd.accept(addr);


	fd.close();
	accept_fd.close();

}