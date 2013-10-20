/*
 * socket_filter.cpp
 *
 *  Created on: 2013. 10. 18.
 *      Author: tk
 */
#include "stdafx.h"
#include <tdk/event_loop/io/ip/tcp/pipeline/socket_filter.hpp>
#include <tdk/io/buffer_adapter.hpp>
#include <tdk/event_loop/event_loop.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

static void _on_recv_complete( tdk::task* t ) {
	socket_filter* f  = static_cast< socket_filter* >( t->context());
	f->on_recv_complete();
}

socket_filter::socket_filter( SOCKET fd )
	: _fd( fd )
	, _on_recv( _on_recv_complete , this )
	, _recv_buffer(4096)
{

}

socket_filter::~socket_filter() {
	// TODO Auto-generated destructor stub
}

void socket_filter::do_recv( void ) {
	_on_recv.reset();
	DWORD flag = 0;	
	tdk::io::buffer_adapter buf( _recv_buffer.wr_ptr() 
		, _recv_buffer.space());

    if ( WSARecv(	_fd.handle() 
					, const_cast< LPWSABUF >( buf.buffers())
					, buf.size()
                    , nullptr 
                    , &flag 
					, _on_recv.impl()
                    , nullptr ) == SOCKET_ERROR )
    {
        if ( WSAGetLastError() != WSA_IO_PENDING ){
			_on_recv.error( tdk::platform::error());
			pipeline()->loop().execute( &_on_recv );
		}
    }
	pipeline()->loop().add_active();
}

void socket_filter::on_connect( connect_event& evt ) {
	do_recv();
}

void socket_filter::on_recv_complete( void ) {
	pipeline()->loop().remove_active();

	if ( closed() ) 
		return;

	if ( _on_recv.error() ) {
		pipeline()->fire_on_error( _on_recv.error() );
	} else {
		_recv_buffer.wr_ptr( _on_recv.io_bytes());
		pipeline()->fire_on_recv();
	}
}

void socket_filter::on_recv( recv_event& evt ) {
	evt.add_message( message( 0 , _recv_buffer ));
	_recv_buffer = tdk::buffer::memory_block( 4096 );
	do_recv();
}

void socket_filter::on_error( error_event& evt ) {
	if ( closed() ) {
		evt.propagation( false );
	}
}

void socket_filter::on_close( close_event& e ) {
	if ( closed() ) {
		e.propagation( false );
	} else {
		_fd.close();
	}
}

bool socket_filter::closed( void ) {
	if ( _fd.handle() == INVALID_SOCKET ) 
		return true;
	return false;
}

void socket_filter::send( const std::vector< message >& msg ) {
	
}

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */
