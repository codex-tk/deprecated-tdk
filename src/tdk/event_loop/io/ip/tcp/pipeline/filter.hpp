/*
 * filter.hpp
 *
 *  Created on: 2013. 10. 18.
 *      Author: tk
 */

#ifndef FILTER_HPP_
#define FILTER_HPP_

#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline_event.hpp>

namespace tdk {
namespace io {
namespace ip {
class address;
namespace tcp {

class pipeline;
class filter {
public:
	filter();
	virtual ~filter();

	tcp::pipeline* pipeline( void );
	void pipeline( tcp::pipeline* p );

	filter* in_bound( void );
	filter* out_bound( void );

	void in_bound( filter* f );
	void out_bound( filter* f );
	void write_out_bound( tcp::message& msg );
public:
	virtual void on_accepted( const tdk::io::ip::address& addr );
	virtual void on_connected( void );
	virtual void on_error( const std::error_code& ec );
	virtual void on_read( tcp::message& msg );
	virtual void on_closed( void );
	virtual void do_write( tcp::message& msg );
	virtual void on_write( int write , bool flushed );
private:
	tcp::pipeline* _pipeline;
	filter* _in_bound;
	filter* _out_bound;
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* FILTER_HPP_ */
