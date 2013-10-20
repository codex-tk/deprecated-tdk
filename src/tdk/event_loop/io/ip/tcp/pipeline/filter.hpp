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
namespace tcp {

class pipeline;
class filter {
public:
	filter();
	virtual ~filter();

	tcp::pipeline* pipeline( void );
	void pipeline( tcp::pipeline* p );

	void bind_filter( filter* prev , filter* next );
	filter* prev( void );
	filter* next( void );

	virtual void on_connect( connect_event& evt ) = 0;
	virtual void on_recv( recv_event& evt ) = 0;
	virtual void on_error( error_event& evt ) = 0;
	virtual void on_close( close_event& evt ) = 0;

	virtual void send( const std::vector< message >& msg );
private:
	tcp::pipeline* _pipeline;

	filter* _prev;
	filter* _next;
};

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* FILTER_HPP_ */
