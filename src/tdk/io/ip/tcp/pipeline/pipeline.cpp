#include "stdafx.h"
#include <tdk/io/ip/tcp/pipeline/pipeline.hpp>
#include <tdk/io/ip/tcp/pipeline/filter.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

pipeline::pipeline( tcp::channel* c )
	: _channel(c)
{
}

pipeline::~pipeline( void ) {

}


void pipeline::add( filter* f , const std::string& name ){
	f->name(name);
	f->channel( _channel );
	_chain.add(f);
}

tcp::channel* pipeline::channel( void ) {
	return _channel;
}

filter* pipeline::in_bound_filter( void ) {
	return _chain.in_bound_filter();
}

filter* pipeline::out_bound_filter( void ) {
	return _chain.out_bound_filter();
}

filter* pipeline::find( const std::string& name ) {
	filter* f = _chain.in_bound_filter();
	while ( f ) {
		if ( f->name() == name ) 
			break;
		f = f->in_bound();
	}
	return f;
}

}}}}
