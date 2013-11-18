#ifndef __tdk_udp_pipeline_h__
#define __tdk_udp_pipeline_h__

#include <tdk/io/ip/socket.hpp>
#include <tdk/io/task.hpp>
#include <tdk/io/ip/udp/pipeline/filter_chain.hpp>
#include <tdk/util/rc_ptr.hpp>
#include <atomic>
#include <list>

namespace tdk {
class event_loop;
namespace io {
namespace ip {
namespace udp {
/*
class channel;
class pipeline {
public:
	pipeline( udp::channel* channel );
	~pipeline( void );

	void add( const std::string& name  , filter* f );

	udp::channel* channel( void );

	filter* in_bound_filter( void );
	filter* out_bound_filter( void );

	filter* find( const std::string& name );
private:
	udp::channel* _channel;
	filter_chain _chain;
};
*/
}}}}

#endif
