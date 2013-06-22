#ifndef __tdk_threading_auto_reset_event_h__
#define __tdk_threading_auto_reset_event_h__

#include <tdk/time/time_span.hpp>

namespace tdk {
namespace threading {

class auto_reset_event {
public:
	explicit auto_reset_event( bool init_state = false );
	~auto_reset_event( void ) ;

	bool wait( const tdk::time_span& w );
	void set( void );
	void reset( void );
private:
	HANDLE _handle;
};

}}


#endif