#ifndef __tdk_io_io_tcp_connector_h__
#define __tdk_io_io_tcp_connector_h__

#include <tdk/io/ip/address.hpp>
#include <tdk/event_loop/io/task.hpp>
#include <tdk/util/rc_ptr.hpp>
#include <tdk/io/ip/socket.hpp>
#include <tdk/event_loop/event_loop.hpp>
#include <vector>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class pipeline_builder;
class pipeline_connector {
public:
	pipeline_connector( tdk::event_loop& l );
	virtual ~pipeline_connector( void );

	void connect( 
		const std::vector< tdk::io::ip::address >& addrs
		, pipeline_builder* builder );
	void connect(
			const std::vector< tdk::io::ip::address >& addrs
			, pipeline_builder* builder
			, const tdk::time_span& time );

	bool connect( const tdk::io::ip::address& adr );

	void on_connect_handler(void);
	void on_timer_handler( void );
	static void _on_connect_handler(tdk::task* t);
	static void _on_timer_handler(tdk::task* t);

	virtual bool on_connnect( const tdk::io::ip::address& addr );
	virtual void on_connect_fail( const std::error_code& ec ) = 0;
private:
	tdk::event_loop& _loop;
	tdk::io::ip::socket _fd;
	pipeline_builder* _builder;
	std::vector< tdk::io::ip::address > _addrs;
	tdk::io::task _on_connect;
	std::size_t _addr_index;
	tdk::timer_task _on_timer;
	tdk::time_span _time_out;
};

}}}}

#endif
