#include "stdafx.h"
#include <tdk/io/engine_win32.hpp>
#include <tdk/io/ip/tcp/socket_win32.hpp>
#include <tdk/io/ip/tcp/operation/connect_operation_win32.hpp>
#include <tdk/io/ip/tcp/operation/send_operation_win32.hpp>
#include <tdk/io/ip/tcp/operation/recv_operation_win32.hpp>
#include <tdk/io/ip/tcp/operation/accept_operation_win32.hpp>

namespace tdk {
namespace io {
namespace detail {

static int k_operation_posted = -1;
static void on_port_callback ( 
    const tdk::error_code& code 
        , int io_byte 
        , void* 
        , OVERLAPPED* ov
        , void* ctx )
{
    operation* op( static_cast< operation* >(ov));
    if ( io_byte == k_operation_posted ) {
        (*op)( op->error() , op->io_bytes() );
    }else {
        (*op)( code , io_byte );	
    }
}

}

engine::engine( void ) {

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

bool engine::post( tdk::io::operation* op , const tdk::error_code& ec ){
    return false;
}

bool engine::bind( SOCKET fd ) {
	return _port.bind( fd , (void*)fd ); 
}

void engine::async_connect( tdk::io::ip::tcp::connect_operation* op ){
    if ( !op->socket().open_tcp( op->address().family() )) {
		return error_handler( op );
	}

    if ( bind( op->socket().handle())){
        return error_handler( op );
    }
    tdk::io::ip::address bind_addr( tdk::io::ip::address::any( 0 , op->address().family()));
    if ( !op->socket().bind( bind_addr ) ) {
		return error_handler( op );
    }
    DWORD dwbytes = 0;
    LPFN_CONNECTEX fp_connect_ex = nullptr;
        
    GUID guid = WSAID_CONNECTEX;
    if ( ( WSAIoctl(   
            op->socket().handle() ,
            SIO_GET_EXTENSION_FUNCTION_POINTER , 
            &guid , 
            sizeof( GUID ) , 
            &fp_connect_ex , 
            sizeof( LPFN_CONNECTEX ) , 
            &dwbytes,
            nullptr , 
            nullptr ) == SOCKET_ERROR ) 
        || ( fp_connect_ex == nullptr )) 
    {
        return error_handler( op );
    }
    op->reset();
    dwbytes = 0;
    if ( fp_connect_ex( op->socket().handle() ,  
                        op->address().sockaddr() , 
                        op->address().sockaddr_length() , 
                        nullptr , 
                        0 ,
                        &dwbytes ,
                        op ) == FALSE ) 
    {
        if ( WSAGetLastError() != WSA_IO_PENDING ){
            return error_handler( op );
        }
    }
    return;
}

void engine::async_send( tdk::io::ip::tcp::send_operation* op ){
	op->reset();
	DWORD flag	= 0;
    if ( WSASend(	op->socket().handle() 
					, const_cast< WSABUF* >(op->buffer_adapter().buffers())
					, op->buffer_adapter().count()
                    , nullptr 
                    , flag 
                    , op
                    , nullptr ) == SOCKET_ERROR )
    {
		if ( WSAGetLastError() != WSA_IO_PENDING ){
			return error_handler( op );
		}
    }
}

void engine::async_recv( tdk::io::ip::tcp::recv_operation* op ){
	op->reset();
	DWORD flag = 0;
    if ( WSARecv(	op->socket().handle() 
                    , const_cast< WSABUF* >(op->buffer_adapter().buffers())
					, op->buffer_adapter().count()
                    , nullptr 
                    , &flag 
                    , op
                    , nullptr ) == SOCKET_ERROR )
    {
        if ( WSAGetLastError() != WSA_IO_PENDING ){
			return error_handler( op );
		}
    }
}

void engine::async_accept( tdk::io::ip::tcp::accept_operation* op ) {
	op->reset();
	if ( !op->socket().open_tcp( op->listen_socket().local_address().family())){
		return error_handler( op );
	}
	DWORD dwBytes = 0;
	if ( AcceptEx(  op->listen_socket().handle() ,
                    op->socket().handle() ,
                    op->address_buffer() ,
                    0 ,
                    sizeof( sockaddr_storage ) ,
                    sizeof( sockaddr_storage ) ,
                    &dwBytes , 
                    op ) == FALSE ) 
    {
		tdk::error_code ec = tdk::platform::error();
        if ( WSAGetLastError() != WSA_IO_PENDING ){
			return error_handler( op );
		}
    }
}

}}
