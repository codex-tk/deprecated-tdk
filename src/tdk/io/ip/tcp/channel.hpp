#ifndef __tdk_io_ip_tcp_channel_h__
#define __tdk_io_ip_tcp_channel_h__

#if defined( _WIN32 )
#include <tdk/io/ip/tcp/channel_win32.hpp>
#else
#include <tdk/io/ip/tcp/channel_linux.hpp>
#endif

#endif