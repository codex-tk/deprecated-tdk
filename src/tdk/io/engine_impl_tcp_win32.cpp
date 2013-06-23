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

void engine::async_connect( tdk::io::ip::tcp::connect_operation* op ){
	inc_posted();
    if ( !op->socket().open_tcp( op->address().family() )) {
		return detail::error_handler( this ,  op );
	}

    if ( !bind( op->socket().handle())){
        return detail::error_handler( this ,  op );
    }
    tdk::io::ip::address bind_addr( tdk::io::ip::address::any( 0 , op->address().family()));
    if ( !op->socket().bind( bind_addr ) ) {
		return detail::error_handler( this ,  op );
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
        return detail::error_handler( this ,  op );
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
            return detail::error_handler( this ,  op );
        }
    }
    return;
}

void engine::async_send( tdk::io::ip::tcp::send_operation* op ){
	inc_posted();
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
			return detail::error_handler( this ,  op );
		}
    }
}

void engine::async_recv( tdk::io::ip::tcp::recv_operation* op ){
	inc_posted();
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
			return detail::error_handler( this ,  op );
		}
    }
}

void engine::async_accept( tdk::io::ip::tcp::accept_operation* op ) {
	inc_posted();
	op->reset();
	if ( !op->socket().open_tcp( op->listen_socket().local_address().family())){
		return detail::error_handler( this ,  op );;
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
			return detail::error_handler( this ,  op );
		}
    }
}

}}
