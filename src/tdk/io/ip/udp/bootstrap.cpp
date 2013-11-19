#include "stdafx.h"
#include <tdk/io/ip/udp/bootstrap.hpp>
#include <tdk/io/ip/udp/channel.hpp>
#include <tdk/io/ip/udp/pipeline/pipeline_builder.hpp>
#include <tdk/io/ip/socket.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace udp {

bootstrap::bootstrap( tdk::event_loop& l )
	: _loop(l)
{

}

bootstrap::~bootstrap( void ) {

}

bool bootstrap::start( int af 
	, tdk::io::ip::udp::pipeline_builder* builder )
{
	tdk::io::ip::socket fd;
	if ( fd.open_udp(af)){
		tdk::io::ip::socket::option::non_blocking non_block;
		fd.set_option(non_block);
#if defined( _WIN32 )
		_loop.io_impl().register_handle( fd.handle() , this );
#endif
		tdk::io::ip::udp::channel* c =
						new tdk::io::ip::udp::channel(
								_loop
								, fd.handle());
		if ( builder->build( c->pipeline()) ) {
			fd.close();
			delete c;
			return false;
		} 
		_loop.execute( tdk::task::make_one_shot_task(
			[c]{
				c->fire_on_open();
			}));
		return true;
	}
	return false;
}

bool bootstrap::start( const tdk::io::ip::address& addr 
	, tdk::io::ip::udp::pipeline_builder* builder )
{
	tdk::io::ip::socket fd;
	if ( fd.open_udp(addr.family())){
		tdk::io::ip::socket::option::non_blocking non_block;
		fd.set_option(non_block);
		if ( fd.bind( addr ) ) {
#if defined( _WIN32 )
			_loop.io_impl().register_handle( fd.handle() , this );
#endif
			tdk::io::ip::udp::channel* c =
						new tdk::io::ip::udp::channel(
								_loop
								, fd.handle());
			if ( builder->build( c->pipeline()) ) {
				fd.close();
				delete c;
				return false;
			} 
			_loop.execute( tdk::task::make_one_shot_task(
				[c]{
					c->fire_on_open();
				}));
			return true;
		}
		fd.close();
		return false;
	}
	return false;
}

}}}}
