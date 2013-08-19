#include <tdk/util/string.hpp>
#include <stdarg.h>

namespace tdk{
namespace string {

std::string& append_format( std::string& msg , const char* format , ... ) {
	if ( format != nullptr ) {
		char fmt_buffer[4096];
		memset( fmt_buffer , 0x00 , 4096 );
		va_list args;
		va_start( args , format );
		int ret = vsnprintf( fmt_buffer , 4096 , format , args );
		va_end( args );
		if (ret >= 4096 || ret < 0)
			fmt_buffer[4095] = '\0';
		msg += fmt_buffer;
	}
	return msg;
}

}}
