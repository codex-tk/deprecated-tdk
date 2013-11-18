#ifndef __tdk_io_ip_udp_bootstrap_h__
#define __tdk_io_ip_udp_bootstrap_h__

#include <tdk/event_loop/event_loop.hpp>
#include <tdk/io/ip/address.hpp>
#include <tdk/io/ip/udp/pipeline/pipeline_builder.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace udp {

class bootstrap {
public:
	bootstrap( tdk::event_loop& l );
	~bootstrap( void );

	bool start( int af 
		, tdk::io::ip::udp::pipeline_builder* builder );
	bool start( const tdk::io::ip::address& addr 
		, tdk::io::ip::udp::pipeline_builder* builder );
private:
	tdk::event_loop& _loop;
};

}}}}

#endif