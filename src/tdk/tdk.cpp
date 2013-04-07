#include "stdafx.h"
#include <tdk/tdk.hpp>
#include <tdk/threading/thread_data.hpp>

namespace tdk {
namespace option {

bool enable_assert = true;

}

bool init( void ) {
	static tdk::threading::data main_data;	
	tdk::threading::data::set_thread_data( &main_data );
#if defined( _WIN32 ) 
	WSAData ws;
	WSAStartup( MAKEWORD(2,2) , &ws);
#endif
	return true;
}

void cleanup( void ) {

}

}