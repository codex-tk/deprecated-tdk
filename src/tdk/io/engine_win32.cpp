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

}

engine::engine( void ) 
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
	}
}

bool engine::bind( SOCKET fd ) {
	return _port.bind( fd , (void*)fd ); 
}
}}
