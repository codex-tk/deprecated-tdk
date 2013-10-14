/*
 * write_task.cpp
 *
 *  Created on: 2013. 10. 10.
 *      Author: tk
 */

#include <tdk/event_loop/io/ip/tcp/write_task.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

write_task::write_task()
	: channel_task(this){

	// TODO Auto-generated constructor stub

}

write_task::~write_task() {
	// TODO Auto-generated destructor stub
}

write_task::write_task( tdk::task::handler h , void* ctx )
 	 :channel_task( this , h , ctx )
{

}

void write_task::buffers( const tdk::io::buffer_adapter& buf ) {
	_buffer = buf;
}

tdk::io::buffer_adapter& write_task::buffers( void ) {
	return _buffer;
}

tdk::io::buffer_adapter write_task::remain_buffers( void ) {
	tdk::io::buffer_adapter buffer;
	const tdk::io::buffer_adapter::buffer_type* impl = _buffer.buffers();
	int count = _buffer.count();
	std::size_t writed = io_bytes();
	for ( int i = 0 ; i < count ; ++i ) {
		if ( impl[i].iov_len <= writed ) {
			writed -= impl->iov_len;
		} else {
			buffer.push_back( static_cast< uint8_t*>(impl[i].iov_base) + writed
					, impl[i].iov_len - writed );
			writed = 0;
		}
	}
	return buffer;
}

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
