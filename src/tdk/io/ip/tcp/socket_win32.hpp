#ifndef __tdk_io_ip_tcp_socket_h__
#define __tdk_io_ip_tcp_socket_h__

#include <tdk/io/ip/socket.hpp>
#include <tdk/io/engine_win32.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class socket : public tdk::io::ip::socket {
public:
	socket( tdk::io::engine& engine );
	~socket( void );

	template < typename T_handler >
	void async_connect( 
		const std::vector< tdk::io::ip::address >& addr 
		, const T_handler& h ) {

	}
private:
	tdk::io::engine& _engine;
};


}}}}

#endif