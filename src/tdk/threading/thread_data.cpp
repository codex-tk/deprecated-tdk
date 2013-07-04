#include "stdafx.h"
#include <tdk/threading/thread_data.hpp>
#include <tdk/threading/thread_local.hpp>

namespace tdk { 
namespace threading {
namespace detail {

tdk::threading::thread_local< tdk::threading::data* > thread_data;

}

data* this_thread::get_data( void ) {
	return detail::thread_data.get();
}

void  this_thread::set_data( data* d ) {
	return detail::thread_data.set( d );
}

}}