#include "stdafx.h"
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/filter.hpp>

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

void pipeline::add( filter* f ) {
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

}}}}
