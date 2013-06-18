#ifndef __tdk_io_ip_tcp_connect_operation_h__
#define __tdk_io_ip_tcp_connect_operation_h__

#include <tdk/io/operation.hpp>
#include <tdk/io/ip/address.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class socket;

class connect_operation : public tdk::io::operation {
public:
	connect_operation( tdk::io::operation::callback cb 
		, tdk::io::ip::tcp::socket& fd
		, const std::vector< tdk::io::ip::address >& addrs
		 );

	~connect_operation( void );

	bool end_operation( void );

	void process( void );
	tdk::io::ip::tcp::socket& socket( void );
	const tdk::io::ip::address& address( void );
private:
	tdk::io::ip::tcp::socket* _socket;
	std::vector< tdk::io::ip::address > _address;
	std::vector< tdk::io::ip::address >::iterator _current_end_point;
};

}}}}

#endif