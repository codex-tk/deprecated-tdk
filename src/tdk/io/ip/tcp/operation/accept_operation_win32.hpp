#ifndef __tdk_io_ip_tcp_accept_operation_h__
#define __tdk_io_ip_tcp_accept_operation_h__

#include <tdk/io/operation.hpp>
#include <tdk/io/ip/address.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class socket;

class accept_operation : public tdk::io::operation {
public:
	accept_operation( 
		tdk::io::ip::tcp::socket& listenfd
		, tdk::io::ip::tcp::socket& fd
		, tdk::io::operation::callback cb );
	~accept_operation( void );

	bool end_opearation( void );

	void process( void );
	tdk::io::ip::tcp::socket& socket( void );
	tdk::io::ip::tcp::socket& listen_socket( void );
	
	void* address_buffer( void );
private:
	tdk::io::ip::tcp::socket* _listen_socket;
	tdk::io::ip::tcp::socket* _socket;
	char _address_buffer[ sizeof(sockaddr_storage) * 2 ];
};

}}}}

#endif