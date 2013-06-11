#include "stdafx.h"
#include <tdk/io/engine_win32.hpp>
#include <tdk/io/ip/tcp/socket_win32.hpp>

TEST( tdk_io_engine , e ){
	tdk::io::engine e;
	ASSERT_TRUE(e.open());

	tdk::io::ip::tcp::socket fd( e );
}