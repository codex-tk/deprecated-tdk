#include "stdafx.h"
#include <tdk/tdk.hpp>
#include <tdk/threading/thread_data.hpp>


namespace tdk {
namespace option {

bool enable_assert = true;
bool is_init = false;
}

bool init( void ) {
	if ( option::is_init )
		return true;
	static tdk::threading::data main_data;	
	tdk::threading::this_thread::set_data( &main_data );
#if defined( _WIN32 ) 
	WSAData ws;
	WSAStartup( MAKEWORD(2,2) , &ws);
#endif
	option::is_init = true;
	return true;
}

void cleanup( void ) {

}


}