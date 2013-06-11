#ifndef __tdk_io_engine_h__
#define __tdk_io_engine_h__

#include <tdk/io/completion_port_win32.hpp>

namespace tdk {
namespace io {

class engine {
public:
	engine( void );
	~engine( void );

	bool open( void );
	void close( void );

	bool run( const tdk::time_span& wait );
private:
	completion_port _port;
};

}}

#endif