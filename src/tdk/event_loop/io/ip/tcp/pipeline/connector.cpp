#include "stdafx.h"
#include <tdk/event_loop/io/ip/tcp/pipeline/connector.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline_builder.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/socket_filter.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

connector::connector( tdk::event_loop& l ) 
	: _on_connect( connector::_on_io_complete , this )
	, _addr_index(0)
	, _loop(l)
{

}

connector::~connector( void ) {

}

void connector::connect( 
	const std::vector< tdk::io::ip::address >& addrs
	, pipeline_builder* builder )
{
	_addrs = addrs;
	_builder = builder;
	_addr_index = 0;

	while ( _addr_index < _addrs.size() ) {
		if ( connect( _addrs[_addr_index]) )
			return;
		++_addr_index;
	}
	_loop.add_active();
	_on_connect.error( tdk::platform::error());
	_loop.execute( &_on_connect );
}

bool connector::connect( const tdk::io::ip::address& addr ) {
#if defined(_WIN32)
	_fd.close();
	if ( !_fd.open_tcp( addr.family())) {
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
#endif
}

void connector::on_io_complete(void) {
	_loop.remove_active();

	std::error_code ec = _on_connect.error();
	if ( _on_connect.error() ) {
		while ( _addr_index < _addrs.size() ) {
			if ( connect( _addrs[_addr_index]) )
				return;
			++_addr_index;
		}
		on_connect_fail( _on_connect.error()); 
	} else {
		on_connnect( _addrs[_addr_index] );
		tdk::io::ip::tcp::pipeline* p = 
			new tdk::io::ip::tcp::pipeline( _loop  );
		p->add_last( new socket_filter( _fd.handle( INVALID_SOCKET ))); 
		std::error_code ec = _builder->build( *p );
		if ( ec ) {
			// delete
			_fd.close();
		} else {
			// open
		}
		
	}
}

void connector::_on_io_complete(tdk::task* t) {
	connector* c = static_cast< connector* >(t->context());
	c->on_io_complete();
}

void connector::on_connnect( const tdk::io::ip::address& addr ) {

}


}}}}