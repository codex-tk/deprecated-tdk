#ifndef __tdk_udp_string_writer_h__
#define __tdk_udp_string_writer_h__

#include <memory>
#include <tdk/log/writer/writer.hpp>
#include <tdk/io/ip/socket.hpp>

namespace tdk {
namespace log {

class udp_writer : public writer {
public:
	udp_writer( const tdk::io::ip::address& endpoint , const tdk::log::formatter_ptr& fmt );
	virtual ~udp_writer( void );
	virtual void write( const record& r );
private:
	tdk::io::ip::address _endpoint;
	tdk::io::ip::socket _socket;
	tdk::buffer::memory_block _buffer;
};

}}


#endif