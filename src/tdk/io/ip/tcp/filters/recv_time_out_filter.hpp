#ifndef __time_out_filter_h__
#define __time_out_filter_h__

#include <tdk/io/ip/tcp/pipeline/filter.hpp>
#include <tdk/event_loop/timer_task.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class recv_time_out_filter : public tdk::io::ip::tcp::filter {
public:
	recv_time_out_filter( const tdk::time_span& recv 
		, const tdk::time_span& tick  = tdk::time_span::from_seconds(5));
	virtual ~recv_time_out_filter( void );

	virtual void on_accepted( const tdk::io::ip::address& addr );
	virtual void on_connected( void );
	virtual void on_error( const std::error_code& ec );
	virtual void on_read( tdk::buffer::memory_block& msg );

	void reg_timer( void );
	void on_timer( void );
private:
	tdk::time_span _recv;
	tdk::time_span _tick;
	tdk::date_time _last_recv;
	tdk::timer_task _on_timer;
};

}}}}


#endif