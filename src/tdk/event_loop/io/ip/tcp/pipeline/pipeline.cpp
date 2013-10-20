#include "stdafx.h"
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/filter.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline_event.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

pipeline::event::event( void ) 
	: _propagation( true ) {
}

pipeline::event::~event( void ) {

}

bool pipeline::event::propagation( void ) {
	return _propagation;
}

void pipeline::event::propagation( bool b ) {
	_propagation = b;
}

pipeline::pipeline( tdk::event_loop& loop  )
	: _loop(loop) 
	, _begin( nullptr )
	, _end( nullptr )
{

}

pipeline::~pipeline( void ) {

}

tdk::event_loop& pipeline::loop( void ) {
	return _loop;
}

void pipeline::add_last( filter* f ) {
	if ( _begin == nullptr ) {
		_begin = _end = f;
	} else {
		_end->bind_filter( _end->prev() , f );
		f->bind_filter( _end , nullptr );
		_end = f;
	}
	f->pipeline( this );
}

void pipeline::fire_on_connect( void ) {
	tcp::connect_event evt;
	filter* f = _begin;
	while ( f != nullptr ) {
		f->on_connect( evt );
		if ( !evt.propagation() ) {
			return;
		}
		f = f->next();
	}
}

void pipeline::fire_on_recv( void ) {
	tcp::recv_event evt;
	filter* f = _begin;
	while ( f != nullptr ) {
		f->on_recv( evt );
		if ( !evt.propagation() ) {
			return;
		}
		f = f->next();
	}
}

void pipeline::fire_on_error( const std::error_code& ec ) {
	tcp::error_event evt;
	evt.error( ec );
	filter* f = _begin;
	while ( f != nullptr ) {
		f->on_error( evt );
		if ( !evt.propagation() ) {
			return;
		}
		f = f->next();
	}
}

void pipeline::fire_on_close( void ){
	tcp::close_event evt;
	filter* f = _begin;
	while ( f != nullptr ) {
		f->on_close( evt );
		if ( !evt.propagation() ) {
			return;
		}
		f = f->next();
	}
}
	


}}}}
