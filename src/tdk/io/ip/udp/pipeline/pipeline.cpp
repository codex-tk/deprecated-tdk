#include "stdafx.h"
#include <tdk/io/ip/udp/pipeline/pipeline.hpp>
#include <tdk/io/ip/udp/pipeline/filter.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace udp {

pipeline::pipeline( udp::channel* c )
	: _channel(c)
{
}

pipeline::~pipeline( void ) {

}


void pipeline::add( const std::string& name  , filter* f ){
	f->name(name);
	f->channel( _channel );
	_chain.add(f);
}

udp::channel* pipeline::channel( void ) {
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
