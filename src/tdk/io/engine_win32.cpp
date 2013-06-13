#include "stdafx.h"
#include <tdk/io/engine_win32.hpp>
#include <tdk/io/ip/tcp/socket_win32.hpp>
#include <tdk/io/ip/tcp/operation/connect_operation_win32.hpp>
#include <tdk/io/ip/tcp/operation/send_operation_win32.hpp>
#include <tdk/io/ip/tcp/operation/recv_operation_win32.hpp>
#include <tdk/io/ip/tcp/operation/accept_operation_win32.hpp>
#include <set>

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

class queue_timer {
public:
	queue_timer( void )
		: _timer( INVALID_HANDLE_VALUE )
	{
		if ( !CreateTimerQueueTimer( &_timer
			, nullptr 
			, &queue_timer::_on_timer 
			, this 
			, 0
			, 1000  
			, WT_EXECUTEDEFAULT ))
		{
		}
	}

	~queue_timer(void){
		DeleteTimerQueueTimer( nullptr , _timer , INVALID_HANDLE_VALUE );
	}

	void on_timer( void ){
		tdk::threading::scoped_lock<> gaurd( _lock );
		for ( auto it : _engines ) {
			it->on_timer();
		}
	}

	void set_timer_handling( tdk::io::engine* e ){
		tdk::threading::scoped_lock<> gaurd( _lock );
		_engines.insert( e );
	}

	void disable_timer_handling( tdk::io::engine* e ) {
		tdk::threading::scoped_lock<> gaurd( _lock );
		_engines.erase( e );
	}

	static void __stdcall _on_timer( void* p , BOOLEAN timer_or_wait_fired ) {
		queue_timer* qt( static_cast< queue_timer* >( p ));
		if ( qt ) 
			qt->on_timer();		
	}

	static queue_timer& instance( void ) {
		static queue_timer timer;
		return timer;
	}
private:
	HANDLE _timer;
	tdk::threading::spin_lock _lock;
	std::set< tdk::io::engine* > _engines;
};

}

engine::engine( void ) 
	: _timer_in_progress(false)
{
	
}

engine::~engine( void ) {
	
}

bool engine::open( void ) {
	_timer_op = new timer_opeartion( *this );
	detail::queue_timer::instance().set_timer_handling( this );
    return _port.create();
}

void engine::close( void ) {	
	detail::queue_timer::instance().disable_timer_handling(this);
    _port.close();
	delete _timer_op;
}

bool engine::run( const tdk::time_span& wait ) {
    return _port.wait( wait , &detail::on_port_callback , this ) >= 0;
}

void engine::post( tdk::io::operation* op , const tdk::error_code& ec ){
	op->error( ec );
	if (!_port.post( detail::k_posted_operation , nullptr , op )){
		tdk::threading::scoped_lock<> guard( _lock );
		_op_queue.add_tail( op );
	}
}

bool engine::bind( SOCKET fd ) {
	return _port.bind( fd , (void*)fd ); 
}

void engine::async_connect( tdk::io::ip::tcp::connect_operation* op ){
    if ( !op->socket().open_tcp( op->address().family() )) {
		return error_handler( op );
	}

    if ( !bind( op->socket().handle())){
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

// main timer tick
void engine::on_timer( void ) {
	tdk::threading::scoped_lock<> guard( _lock );
	if ( _timer_in_progress )
		return;

	if ( _op_queue.is_empty() ) 
		return ;

	if ( _port.post( detail::k_posted_operation , nullptr , _timer_op ))
		_timer_in_progress = true;
};


void engine::drain( void ) {
	tdk::threading::scoped_lock<> guard( _lock );
	while ( !_op_queue.is_empty() ){
		tdk::io::operation* op = _op_queue.front();
		_op_queue.pop_front();
		if ( !_port.post( detail::k_posted_operation , nullptr, op)) {
			_op_queue.add_front( op );
			_timer_in_progress = false;
			return;
		}
	}
	_timer_in_progress = false;
}

engine::timer_opeartion::timer_opeartion( engine& e ) 
	: _engine(e)
	, tdk::io::operation( &engine::timer_opeartion::_on_complete )
{

}

engine::timer_opeartion::~timer_opeartion( void ) {

}

void engine::timer_opeartion::on_complete( void ) {
	_engine.drain();
}

void engine::timer_opeartion::_on_complete( tdk::io::operation* op ) {
	engine::timer_opeartion* impl( static_cast< engine::timer_opeartion* >(op));
	impl->on_complete();
}
}}
