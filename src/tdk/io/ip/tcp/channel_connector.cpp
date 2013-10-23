#include "stdafx.h"
#include <tdk/io/ip/tcp/channel_connector.hpp>
#include <tdk/io/ip/tcp/channel.hpp>
#include <tdk/io/ip/tcp/pipeline/pipeline_builder.hpp>
#include <tdk/error/error_platform.hpp>
#include <tdk/error/error_tdk.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

channel_connector::channel_connector( tdk::event_loop& l )
	: _loop(l)
	, _builder( nullptr )
	, _on_connect( channel_connector::_on_connect_handler , this )
	, _addr_index(0)
	, _on_timer( channel_connector::_on_timer_handler , this )
	, _time_out( 0 )
{

}

channel_connector::~channel_connector( void ) {

}

void channel_connector::reconnect(void) {
	_addr_index = 0;

	while (_addr_index < _addrs.size()) {
		if (connect(_addrs[_addr_index++]))
			return;
	}
	_loop.add_active();
	_on_connect.error(tdk::platform::error());
	_loop.execute(&_on_connect);
}

void channel_connector::connect(
	const std::vector< tdk::io::ip::address >& addrs
	, pipeline_builder* builder )
{
	_addrs = addrs;
	_builder = builder;
	_addr_index = 0;

	while ( _addr_index < _addrs.size() ) {
		if ( connect( _addrs[_addr_index++]) )
			return;
	}
	_loop.add_active();
	_on_connect.error( tdk::platform::error());
	_loop.execute( &_on_connect );
}

void channel_connector::connect(
			const std::vector< tdk::io::ip::address >& addrs
			, pipeline_builder* builder
			, const tdk::time_span& time )
{
	_time_out = time;
	connect( addrs , builder );
}

bool channel_connector::connect( const tdk::io::ip::address& addr ) {
#if defined(_WIN32)
	_fd.close();
	if ( !_fd.open_tcp( addr.family())) {
		return false;
	}

	tdk::io::ip::socket::option::non_blocking nb;
	if ( !_fd.set_option( nb )) {
		return false;
	}

	if ( !_loop.io_impl().register_handle( _fd.handle() 
		, reinterpret_cast<void*>(_fd.handle())))
	{
        return false;
    }
    tdk::io::ip::address bind_addr( tdk::io::ip::address::any( 0 , addr.family()));
    if ( !_fd.bind( bind_addr ) ) {
		return false;
    }
    DWORD dwbytes = 0;
    LPFN_CONNECTEX fp_connect_ex = nullptr;
        
    GUID guid = WSAID_CONNECTEX;
    if ( ( WSAIoctl(   
            _fd.handle() ,
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
        return false;
    }
    dwbytes = 0;
	_on_connect.reset();
    if ( fp_connect_ex( _fd.handle() ,  
                        addr.sockaddr() , 
                        addr.sockaddr_length() , 
                        nullptr , 
                        0 ,
                        &dwbytes ,
						_on_connect.impl()) == FALSE ) 
    {
        if ( WSAGetLastError() != WSA_IO_PENDING ){
            return false;
        }
    }
	_loop.add_active();
    return true;
#else
    _fd.close();
	if ( _fd.open_tcp(addr.family())){
		tdk::io::ip::socket::option::non_blocking nb;
		if ( _fd.set_option( nb )){
			int r = 0;
			do {
				r = ::connect(_fd.handle()
						, addr.sockaddr()
						, addr.sockaddr_length());
			}while((r==-1)&&(errno==EINTR));
			if (( r == 0 ) || errno == EINPROGRESS ) {
				_on_connect.evt( EPOLLOUT | EPOLLONESHOT );
				if (_loop.io_impl().register_handle(
						_fd.handle()
						, &_on_connect ))
				{
					_loop.add_active();
					if ( _time_out.delta() != 0 ) {
						_on_timer.expired_at(tdk::date_time::utc() + _time_out );
						_loop.schedule(&_on_timer );
					}
					return true;
				}
			}
		}
	}
	return false;
#endif
}

void channel_connector::on_timer_handler(void) {
	_fd.close();
	_loop.remove_active();
	while ( _addr_index < _addrs.size() ) {
		if ( connect( _addrs[_addr_index++]) )
			return;
	}
	on_connect_fail( tdk::platform::error(ETIMEDOUT));
}

void channel_connector::on_connect_handler(void) {
#if !defined( _WIN32 )
	if ( _on_connect.evt() & EPOLLOUT ) {
		_on_connect.error(std::error_code());
		tdk::io::ip::socket::option::error err;
		if ( _fd.get_option(err) && err.value()!= 0 ){
			_on_connect.error(tdk::platform::error( err.value()));
		}
	} else {
		if ( _on_connect.evt() & EPOLLERR ) {
			_on_connect.error( tdk::tdk_epoll_error );
			tdk::io::ip::socket::option::error err;
			if ( _fd.get_option(err) && err.value()!= 0 ){
				_on_connect.error(tdk::platform::error( err.value()));
			}
		} else if ( _on_connect.evt() & EPOLLHUP ) {
			_on_connect.error( tdk::tdk_epoll_hang_up );
		} else {
			_on_connect.error( tdk::tdk_invalid_event );
		}
	}
#endif
	if ( _time_out.delta() != 0 ) {
		_loop.cancel(&_on_timer);
	}
	_loop.remove_active();
	if ( _on_connect.error() ) {
		_fd.close();
		while ( _addr_index < _addrs.size() ) {
			if ( connect( _addrs[_addr_index++]) )
				return;
		}
		on_connect_fail( _on_connect.error()); 
	} else {
		if ( on_connnect( _addrs[_addr_index] )) {

			tdk::io::ip::tcp::channel* c =
				new tdk::io::ip::tcp::channel(_loop , _fd.handle() );
			std::error_code ec = _builder->build( c->pipeline() );
			if ( ec ) {
				delete c;
			} else {
				c->fire_on_connected();
			}
		}
	}
}

void channel_connector::_on_connect_handler(tdk::task* t) {
	channel_connector* c = static_cast< channel_connector* >(t->context());
	c->on_connect_handler();
}

void channel_connector::_on_timer_handler(tdk::task* t) {
	channel_connector* c = static_cast< channel_connector* >(t->context());
	c->on_timer_handler();
}

bool channel_connector::on_connnect( const tdk::io::ip::address& addr ) {
	return true;
}

tdk::event_loop& channel_connector::loop( void ) {
	return _loop;
}

}}}}
