#include "stdafx.h"
#include <tdk/io/engine_win32.hpp>
#include <tdk/io/ip/tcp/socket_win32.hpp>
#include <tdk/io/ip/tcp/operation/connect_operation_win32.hpp>
#include <tdk/io/ip/tcp/operation/send_operation_win32.hpp>
#include <tdk/io/ip/tcp/operation/recv_operation_win32.hpp>
#include <tdk/io/ip/tcp/operation/accept_operation_win32.hpp>
#include <tdk/task/queue_timer_win32.hpp>

namespace tdk {
namespace io {
namespace detail {

static int k_posted_operation = -1;

static void on_port_callback ( 
    const tdk::error_code& code 
        , int io_byte 
        , void* 
        , OVERLAPPED* ov
        , void* ctx )
{
    operation* op( static_cast< operation* >(ov));
    if ( io_byte == k_posted_operation ) {
        (*op)( op->error() , op->io_bytes() );
    }else {
        (*op)( code , io_byte );	
    }
}

class drain_operation 
	: public tdk::io::operation 
	, public tdk::task::queue_timer::handler
{
public:
	drain_operation( engine& e )
		: _engine(e)
		, tdk::io::operation( &drain_operation::_on_complete )
	{
		retain();
	}

	virtual ~drain_operation( void ) {

	}

	// timer callback
	virtual void operator()( const tdk::error_code& e) {
		_engine.req_drain_post_fails( this );
	}
	// operation callback
	void on_complete( void ) {
		_engine.do_drain_post_fails();
	}

	static void __stdcall _on_complete( tdk::io::operation* op ) {
		drain_operation* impl( static_cast< drain_operation* >(op));
		impl->on_complete();
		impl->release();
	}
private:
	engine& _engine;
};


}

engine::engine( void ) 
	: _drain_in_progress(false)
{
	
}

engine::~engine( void ) {
	
}

bool engine::open( void ) {
    return _port.create();
}

void engine::close( void ) {	
    _port.close();
}

bool engine::run( const tdk::time_span& wait ) {
    return _port.wait( wait , &detail::on_port_callback , this ) >= 0;
}

void engine::post( tdk::io::operation* op , const tdk::error_code& ec ){
	op->error( ec );
	if (!_port.post( detail::k_posted_operation , nullptr , op )){
		tdk::threading::scoped_lock<> guard( _lock );
		_op_queue.add_tail( op );
		if ( !_drain_in_progress )
			_set_drain_handling( nullptr );
	}
}

bool engine::bind( SOCKET fd ) {
	return _port.bind( fd , (void*)fd ); 
}

void engine::req_drain_post_fails( detail::drain_operation* op ) {
	if ( !_port.post( detail::k_posted_operation , nullptr , op )) {
		_set_drain_handling( op );
	}	
};

void engine::do_drain_post_fails( void ) {
	tdk::io::operation* op = nullptr;
	do {
		do {
			tdk::threading::scoped_lock<> guard( _lock );
			if ( !_op_queue.is_empty() ){
				op = _op_queue.front();
				_op_queue.pop_front();
			} else {
				op = nullptr;
			}
		}while(0);
		if ( op != nullptr ){
			(*op)( op->error() , op->io_bytes() );
		}
	} while ( op != nullptr );
	_drain_in_progress = false;
}

void engine::_set_drain_handling( detail::drain_operation* op ){
	_drain_in_progress = true;
	if ( op == nullptr ) {
		op = new detail::drain_operation( *this );
	}
	tdk::task::queue_timer::timer_id id( op );
	id->expired_at( tdk::date_time::local());			

	tdk::task::queue_timer::instance().schedule( id );
}

}}
