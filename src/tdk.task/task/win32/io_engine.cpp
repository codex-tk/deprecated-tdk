#include "stdafx.h"
#include <tdk.task/task/operation.hpp>
#include <tdk.task/task/win32/io_engine.hpp>
#include <tdk.task/task/event_loop.hpp>
#include <tdk/error/platform_error.hpp>
#include <tdk.task/network/tcp/channel.hpp>
#include <tdk.task/task/event_loop.hpp>

namespace tdk{
namespace task {
namespace detail{
static int posted_operation = 0xffffffff;

static void on_complete( const tdk::error_code& code 
		, int io_byte 
		, void* key
		, OVERLAPPED* ov
		, void* ctx ) 
{
	tdk::task::io_engine* engine = static_cast<tdk::task::io_engine*>(ctx);
	engine->on_complete( code , io_byte , key , ov );
}

}

io_engine::io_engine( event_loop& loop ) 
	: _loop( loop )
{

}

io_engine::~io_engine( void ) {

}

bool io_engine::open( void ) {
	return _port.open( &detail::on_complete , this );
}

void io_engine::close( void ){
	_port.close();
}

bool io_engine::bind( SOCKET fd , void* obj ) {
	return _port.bind( fd , obj );
}


void io_engine::add_accept_io( tdk::network::tcp::accept_operation* op) 
{
	_loop.increment_ref();
	op->reset();	
	if ( !op->socket().open_tcp( op->acceptor()->address().family() )) {
		op->error( platform_error() );
		post( op );
		return;
	}
	DWORD dwBytes = 0;
	if ( AcceptEx(  op->acceptor()->socket().handle() ,
					op->socket().handle() ,
					op->address_ptr() ,
                    0 ,
					sizeof( sockaddr_storage ) ,
                    sizeof( sockaddr_storage ) ,
                    &dwBytes , 
                    op ) == FALSE ) 
    {
		tdk::error_code ec = tdk::platform_error();
		if ( ec.value() != WSA_IO_PENDING ){
			op->error( ec );
			post( op );
		}
    }
}

void io_engine::add_recv_io( tdk::network::tcp::recv_operation* op ) {
	_loop.increment_ref();
	op->reset();
	DWORD flag = 0;
	WSABUF buffer;
	buffer.buf = (CHAR*)op->buffer().wr_ptr();
	buffer.len = op->buffer().space();
	if ( WSARecv(	op->channel()->socket().handle() 
					, &buffer
					, 1
					, nullptr 
					, &flag 
					, op
					, nullptr ) == SOCKET_ERROR )
	{
		tdk::error_code ec = tdk::platform_error();
		if ( ec.value() != WSA_IO_PENDING ){
			op->error( ec );
			post( op );
		}
	}
}

void io_engine::add_send_io( tdk::network::tcp::send_operation* op ) {
	_loop.increment_ref();
	static const int send_buffer_size = 256;

	op->reset();
	WSABUF send_buffer[send_buffer_size];
	int i = 0;
	for ( auto buffer : op->buffers() ) {
		if ( buffer.length() > 0 ) {
			send_buffer[i].buf = (CHAR*)(buffer.rd_ptr());
			send_buffer[i].len = buffer.length();
			++i;
		}
		if ( i >= send_buffer_size ) {
			break;
		}
	}
	if ( i <= 0 ) {
		op->error( platform_error( ERROR_INVALID_PARAMETER ));
		post( op );
		return;
	} 
	DWORD flag	= 0;
	if ( WSASend(	op->channel()->socket().handle() 
					, send_buffer
					, i
					, nullptr 
					, flag 
					, op
					, nullptr ) == SOCKET_ERROR )
	{
		tdk::error_code ec = tdk::platform_error();
		if ( ec.value() != WSA_IO_PENDING ){
			op->error( ec );
			post( op );
		}
	}
}

void io_engine::add_connect_io( tdk::network::tcp::connect_operation* op ) {
	_loop.increment_ref();
	tdk::network::socket fd;
	if ( !fd.open_tcp( op->address().family())) {
		fd.close();
		op->error( platform_error());
		post( op );
		return;
	}

	if ( !op->channel()->open( fd ) ) {
		fd.close();
		op->error( platform_error());
		post( op );
		return;
	}

	tdk::network::address bind_addr = tdk::network::address::any( 0 , op->address().family() );
	if ( !op->channel()->socket().bind( bind_addr ) ) {
		fd.close();
		op->error( platform_error());
		post( op );
		return;
	}

	DWORD dwbytes = 0;
	LPFN_CONNECTEX fp_connect_ex = nullptr;
		
	GUID guid = WSAID_CONNECTEX;
	if ( WSAIoctl(   
			op->channel()->socket().handle() ,
			SIO_GET_EXTENSION_FUNCTION_POINTER , 
			&guid , 
			sizeof( GUID ) , 
			&fp_connect_ex , 
			sizeof( LPFN_CONNECTEX ) , 
			&dwbytes,
			nullptr , 
			nullptr ) == SOCKET_ERROR ) 
	{
		fd.close();
		op->error( platform_error());
		post( op );
		return;
	}
        
	if ( fp_connect_ex == nullptr ) {
		fd.close();
		op->error( platform_error());
		post( op );
		return;
	}
	op->reset();
	dwbytes = 0;
	if ( fp_connect_ex( op->channel()->socket().handle() ,  
						op->address().sockaddr() , 
						op->address().sockaddr_length() , 
						nullptr , 
						0 ,
						&dwbytes ,
						op ) == FALSE ) 
	{
		tdk::error_code ec = tdk::platform_error();
		if ( ec.value() != WSA_IO_PENDING ){
			fd.close();
			op->error( platform_error());
			post( op );
			return;
		}
	}
	return;
}

void io_engine::add_recvfrom_io( tdk::network::udp::recvfrom_operation* op ) {
	_loop.increment_ref();
	op->reset();
	DWORD flag = 0;
	WSABUF buffer;
	buffer.buf = (CHAR*)op->buffer().wr_ptr();
	buffer.len = op->buffer().space();
	if ( WSARecvFrom(	op->channel()->socket().handle() 
					, &buffer
					, 1
					, nullptr 
					, &flag 
					, op->address().sockaddr()
					, op->address().sockaddr_length_ptr()
					, op
					, nullptr ) == SOCKET_ERROR )
	{
		tdk::error_code ec = tdk::platform_error();
		if ( ec.value() != WSA_IO_PENDING ){
			op->error( ec );
			post( op );
		}
	}
}

void io_engine::post( operation* ctx ) {
	// iocp post 는 내부적으로 실패할수 잇으며
	// post 순서가 필요하므로 post 는 내부적인 queue 로 처리한다.
	tdk::threading::scoped_lock<> gaurd( _op_queue_lock );
	_op_queue.add_tail( ctx );
	/*
	if ( !_port.post( detail::posted_operation , nullptr , ctx )) {
		tdk::threading::scoped_lock<> gaurd( _post_fail_lock );
		_op_queue.add_tail( ctx );
		_post_failed.exchange(1);
	}*/
}

bool io_engine::run( const tdk::time_span& wait ) {
	tdk::slist_queue< operation > drains;
	do {
		tdk::threading::scoped_lock< > guard( _op_queue_lock );
		while ( !_op_queue.is_empty() ) {
			drains.add_tail( _op_queue.front() );
			_op_queue.pop_front();
		}
	}while(0);
	while ( !drains.is_empty() ) {
		on_complete( drains.front()->error() 
			, detail::posted_operation
			, drains.front()->object()
			, drains.front() );
		drains.pop_front();
	}
	/*
	if( _post_failed.compare_and_swap( 0 , 1 ) == 1 ) {
		tdk::slist_queue< operation > drains;
		do {
			tdk::threading::scoped_lock< > guard( _post_fail_lock );
			while ( !_op_queue.is_empty() ) {
				drains.add_tail( _op_queue.front() );
				_op_queue.pop_front();
			}
		}while(0);
		while ( !drains.is_empty() ) {
			on_complete( drains.front()->error() 
				, detail::posted_operation
				, drains.front()->object()
				, drains.front() );
			drains.pop_front();
		}
	}*/
	return _port.wait( wait ) >= 0;
}

void io_engine::on_complete( const tdk::error_code& code 
		, int io_byte 
		, void* key
		, OVERLAPPED* ov )
{
	if ( ov != nullptr ) {
		tdk::task::operation* op = static_cast< tdk::task::operation* >( ov );
		if ( io_byte != detail::posted_operation ) {
			op->error( code );
			op->io_byte( io_byte );
			op->object( key );
		}
		(*op)();
		_loop.decrement_ref();
	}
}

}}