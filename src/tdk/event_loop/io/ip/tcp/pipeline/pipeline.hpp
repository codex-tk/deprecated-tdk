#ifndef __tdk_tcp_pipeline_h__
#define __tdk_tcp_pipeline_h__

#include <tdk/io/ip/socket.hpp>
#include <tdk/event_loop/io/task.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/filter_chain.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/config.hpp>
#include <tdk/util/rc_ptr.hpp>
#include <atomic>
#include <list>

namespace tdk {
class event_loop;
namespace io {
namespace ip {
namespace tcp {

class channel;
class pipeline {
public:
	pipeline( tcp::channel* channel );
	~pipeline( void );

	void add( filter* f );

	tcp::channel* channel( void );

	filter* in_bound_filter( void );
	filter* out_bound_filter( void );

private:
	tcp::channel* _channel;
	filter_chain _chain;
};

}}}}

#endif
