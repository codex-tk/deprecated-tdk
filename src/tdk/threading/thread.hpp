#ifndef __tkd_threading_thread_h__
#define __tkd_threading_thread_h__

#include <tdk/tdk.hpp>
#include <tdk/time/time_span.hpp>
#include <tdk/task/handler.hpp>

namespace tdk { namespace threading {

class thread {
public:
	template < typename T_function >
	explicit thread( const T_function& f ) 
		: _handler( f ) {
	}

	template < typename T_function , typename T_object >
	thread( const T_function& f
		,const T_object& obj ) 
		: _handler( f , obj ) {
	}

	~thread( void );

	bool start( void );
	bool stop( const tdk::time_span& wait );

	int run( void );
private:
#if defined( _WIN32 )
	HANDLE _handle;
#else
	pthread_t _handle
#endif
	tdk::task::handler< int () > _handler;
};

}}

#endif