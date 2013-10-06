#ifndef __tdk_event_loop_h__
#define __tdk_event_loop_h__


#if defined ( linux ) || defined( __linux )
#include <tdk/event_loop/io/epoll.hpp>
#elif defined( _WIN32 )
#include <tdk/event_loop/io/iocp.hpp>
#endif

class event_loop{

};

#endif