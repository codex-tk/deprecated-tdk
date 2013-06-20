#ifndef __tdk_threading_mutex_h__
#define __tdk_threading_mutex_h__

#include <tdk/time/time_span.hpp>

namespace tdk {
namespace threading {

class mutex {
public:
	mutex( void );
	~mutex( void );

	bool create( const  wchar_t* name , bool initstate = false );
	bool open( const tdk::tstring& name );
	void close( void );

	bool wait( const tdk::time_span& ts );
	void release( void );
private:
	HANDLE _mutex;
};



}}

#endif