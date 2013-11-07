#include "stdafx.h"
#include <tdk/buffer/marshal.hpp>
#if defined( _WIN32 )
namespace tdk {
namespace buffer {

endian host_endian( void ) {
	static int host = -1;
	if ( host == -1 ) {
		uint16_t tester = 0x01;
		uint8_t* ptester = reinterpret_cast<uint8_t*>( &tester );
		if ( ptester[0] == 1 ) 
			host = static_cast< int >(endian::little);
		else 
			host = static_cast< int >(endian::big);
	}
	return static_cast< endian >(host);
}

}}
#endif
