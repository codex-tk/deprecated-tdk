#ifndef __tdk_io_io_tcp_connector_h__
#define __tdk_io_io_tcp_connector_h__

#include <tdk/io/ip/address.hpp>
#include <tdk/event_loop/io/task.hpp>
#include <tdk/util/rc_ptr.hpp>

#include <vector>
namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class pipeline_builder;
class connector {
public:
	class connect_task 
		: public tdk::io::task 
		, public tdk::rc_ptr_base< connect_task >
	{
	public:

	private:
		SOCKET _fd;
		std::vector< tdk::io::ip::address >& addrs
		pipeline_builder* _builder;
	};

	typedef tdk::rc_tpr<connect_task> connect_task_ptr;

	static connect_task_ptr connect( 
		const std::vector< tdk::io::ip::address >& addrs
		, pipeline_builder* builder );
private:
};

}}}}

#endif