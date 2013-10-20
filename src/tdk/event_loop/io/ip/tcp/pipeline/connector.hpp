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
class connector {
public:
	connector( tdk::event_loop& l );
	virtual ~connector( void );

	void connect( 
		const std::vector< tdk::io::ip::address >& addrs
		, pipeline_builder* builder );

	bool connect( const tdk::io::ip::address& adr );

	void on_io_complete(void);
	static void _on_io_complete(tdk::task* t); 

	virtual void on_connnect( const tdk::io::ip::address& addr ); 
	virtual void on_connect_fail( const std::error_code& ec ) = 0;
private:
	tdk::io::ip::socket _fd;
	pipeline_builder* _builder;
	std::vector< tdk::io::ip::address > _addrs;
	tdk::io::task _on_connect;
	std::size_t _addr_index;
	tdk::event_loop& _loop;
};

}}}}

#endif