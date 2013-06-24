#include "stdafx.h"
#include <tdk/io/engine_win32.hpp>
#include <tdk/io/ip/tcp/socket_win32.hpp>
#include <tdk/io/ip/tcp/operation/connect_operation_win32.hpp>
#include <tdk/io/ip/tcp/operation/send_operation_win32.hpp>
#include <tdk/io/ip/tcp/operation/recv_operation_win32.hpp>
#include <tdk/io/ip/tcp/operation/accept_operation_win32.hpp>
#include <tdk/task/queue_timer_win32.hpp>
#include <tdk/io/engine_scheduler_win32.hpp>

namespace tdk {
namespace io {
namespace detail {

static LONG __stdcall default_engine_exception_handler( EXCEPTION_POINTERS* exp ) {
	return EXCEPTION_EXECUTE_HANDLER;
}

LONG ( __stdcall* engine_exception_handler)( EXCEPTION_POINTERS* ) = &default_engine_exception_handler;

static int k_posted_operation = -1;

static void on_port_callback ( 
    const tdk::error_code& code 
        , int io_byte 
        , void* 
        , OVERLAPPED* ov
        , void* ctx )
{
	tdk::io::engine* e( static_cast< tdk::io::engine* >( ctx ));
	__try {
		e->process( code , io_byte , ov );
	}__except( engine_exception_handler(GetExceptionInformation())){

	}
}

}

engine::engine( void ){
	if ( _port.create() ){
		_scheduler= new engine::scheduler( *this);
	}
}

engine::~engine( void ) {
	if ( _scheduler != nullptr ) {
		delete _scheduler;
	}
    _port.close();
}

void engine::run( void ) {
	while( _posted.load() > 0 ) {
		run_once( tdk::time_span::infinite());
	}
}

bool engine::run_once( const tdk::time_span& wait ) {
    return _port.wait( wait , &detail::on_port_callback , this ) >= 0;
}

void engine::post( tdk::io::operation* op , const tdk::error_code& ec ){
	inc_posted();
	op->error( ec );
	if (!_port.post( detail::k_posted_operation , nullptr , op )){
		_scheduler->post_fail( op );
	}
}

bool engine::bind( SOCKET fd ) {
	return _port.bind( fd , (void*)fd ); 
}
// scheduler post ¿ëµµ
bool engine::post0( tdk::io::operation* op , const tdk::error_code& ec ) {
	op->error( ec );
	inc_posted();
	if ( !_port.post( detail::k_posted_operation , nullptr , op ) ){
		dec_posted();
		return false;
	}
	return true;
}


engine::timer_id engine::schedule( engine::timer_id& id ){
	inc_posted();
	_scheduler->schedule( id );
	return id;
}

void engine::cancel( engine::timer_id& id ) {
	_scheduler->cancel( id );
}

void engine::process( const tdk::error_code& code , int io_byte  , OVERLAPPED* ov ) {
	operation* op( static_cast< operation* >(ov));
	tdk::error_code ec = code;
	if ( io_byte == detail::k_posted_operation ) {
		ec = op->error();
		io_byte = op->io_bytes();
	}
	(*op)( ec , io_byte );
	dec_posted();
}

void engine::inc_posted( void ) {
	_posted.increment();
}

void engine::dec_posted( void ) {
	_posted.decrement();
}

void engine::set_exception_handler( LONG ( __stdcall* exception_handler)( EXCEPTION_POINTERS*  ) ) {
	detail::engine_exception_handler = exception_handler;
}

}}
