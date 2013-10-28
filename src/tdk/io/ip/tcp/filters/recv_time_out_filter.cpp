#include "stdafx.h"
#include <tdk/io/ip/tcp/filters/recv_time_out_filter.hpp>
#include <tdk/event_loop/event_loop.hpp>
#include <tdk/error/error_platform.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

void on_timer_handler( tdk::task* t ){
	recv_time_out_filter* f = static_cast< recv_time_out_filter* >(t->context());
	f->on_timer();
}

recv_time_out_filter::recv_time_out_filter( const tdk::time_span& recv 
	, const tdk::time_span& tick )
	: _recv( recv )
	, _tick( tick )
	, _on_timer( &on_timer_handler , this )
{

}
recv_time_out_filter::~recv_time_out_filter( void ){

}

void recv_time_out_filter::reg_timer( void ) {
	_on_timer.expired_at( tdk::date_time::utc() + _tick );
	channel()->loop().schedule( &_on_timer );
}


void recv_time_out_filter::on_accepted( const tdk::io::ip::address& addr ){
	_last_recv = tdk::date_time::utc();
	reg_timer();
	filter::on_accepted(addr);
}

void recv_time_out_filter::on_connected( void ){
	_last_recv = tdk::date_time::utc();
	reg_timer();
	filter::on_connected();
}

void recv_time_out_filter::on_error( const std::error_code& ec ){
	channel()->loop().cancel( &_on_timer );
	filter::on_error(ec);
}

void recv_time_out_filter::on_read( tdk::buffer::memory_block& msg ){
	_last_recv = tdk::date_time::utc();
	filter::on_read(msg);
}

void recv_time_out_filter::on_timer( void ) {
	tdk::time_span diff = tdk::date_time::utc() - _last_recv;
	if ( diff > _recv ) {
		channel()->error_propagation( tdk::platform::timeout );
	} else {
		reg_timer();
	}
}

}}}}

