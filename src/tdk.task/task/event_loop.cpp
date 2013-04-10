#include "stdafx.h"
#include <tdk.task/task/event_loop.hpp>

namespace tdk {
namespace task {

event_loop::event_loop()
	: _engine( *this )
{

}

event_loop::~event_loop(){

};

bool event_loop::open( void ){
	return _engine.open();
}

void event_loop::close( void ) {
	_engine.close();
}

io_engine& event_loop::engine( void ){ 
	return _engine;
}

void event_loop::run( void ) {
	return ;
}

bool event_loop::run_once( const tdk::time_span& wait ) {
	return _engine.run( wait );
}

void event_loop::set_on_accept( on_accept cb , void* ctx ) {
	_on_accept.first = cb;
	_on_accept.second = ctx;
}

void event_loop::set_on_recv( on_recv cb , void* ctx ) {
	_on_recv.first = cb;
	_on_recv.second = ctx;
}


void event_loop::on_connect_complete( void ){}
void event_loop::on_send_complete( void ){}

void event_loop::on_recv_complete( const tdk::error_code& code
		, tdk::network::tcp::channel& channel
		, tdk::buffer::memory_block& mb )
{
	if ( _on_recv.first ) {
		_on_recv.first( code , channel , mb , _on_recv.second );
	} else {
		channel.close();
	}
}

void event_loop::on_accept_complete( const tdk::error_code& code
		, tdk::network::tcp::acceptor& acceptor
		, tdk::network::socket fd )
{
	if ( _on_accept.first ) {
		_on_accept.first( code , acceptor , fd , _on_accept.second );
	} else {
		fd.close();
	}
}

}}
