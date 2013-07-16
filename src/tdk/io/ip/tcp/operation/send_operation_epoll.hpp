#ifndef __tdk_io_ip_tcp_send_operation_h__
#define __tdk_io_ip_tcp_send_operation_h__

#include <tdk/io/operation.hpp>
#include <tdk/io/buffer_adapter.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class socket;

class send_operation : public tdk::io::operation {
public:
	send_operation(  tdk::io::operation::callback cb 
		, tdk::io::ip::tcp::socket& fd
		, const tdk::io::buffer_adapter& buffer	);
	~send_operation( void );

	bool end_operation( void );

	void process( void );
	tdk::io::ip::tcp::socket& socket( void );

	const tdk::io::buffer_adapter& buffer_adapter( void );
private:
	tdk::io::ip::tcp::socket* _socket;
	tdk::io::buffer_adapter _buffer_adapter;
};

}}}}

#endif
