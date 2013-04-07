#include "stdafx.h"
#include <tdk/threading/thread_data.hpp>
#include <tdk/threading/thread_local.hpp>

namespace tdk { 
namespace threading {
namespace detail {

tdk::threading::thread_local< tdk::threading::data* > thread_data;

}

data* data::get_thread_data( void ) {
	return detail::thread_data.get();
}

void  data::set_thread_data( data* d ) {
	return detail::thread_data.set( d );
}

}}