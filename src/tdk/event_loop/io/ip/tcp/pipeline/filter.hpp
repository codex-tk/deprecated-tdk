/*
 * filter.hpp
 *
 *  Created on: 2013. 10. 18.
 *      Author: tk
 */

#ifndef FILTER_HPP_
#define FILTER_HPP_

#include <tdk/event_loop/io/ip/tcp/channel.hpp>

namespace tdk {
namespace io {
namespace ip {
class address;
namespace tcp {

class filter {
public:
	filter();
	virtual ~filter();

	tcp::channel* channel( void );
	void channel( tcp::channel* p );

	filter* in_bound( void );
	filter* out_bound( void );

	void in_bound( filter* f );
	void out_bound( filter* f );
	void write_out_bound( tdk::buffer::memory_block& msg );
public:
	virtual void on_accepted( const tdk::io::ip::address& addr );
	virtual void on_connected( void );
	virtual void on_error( const std::error_code& ec );
	virtual void on_read( tdk::buffer::memory_block& msg );
	virtual void on_closed( void );
	virtual void do_write( tdk::buffer::memory_block& msg );
	virtual void on_write( int write , bool flushed );
	virtual void on_delete( void );
private:
	tcp::channel* _channel;
	filter* _in_bound;
	filter* _out_bound;
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* FILTER_HPP_ */
