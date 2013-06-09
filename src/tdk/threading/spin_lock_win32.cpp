#include "stdafx.h"
#include <tdk/threading/spin_lock.hpp>

namespace tdk {
namespace threading {

spin_lock::spin_lock( void ) {
	BOOL ret = InitializeCriticalSectionAndSpinCount( &_lock , 4000 );
	/*
#if defined(_WIN32) || defined(_WIN64)
    
#else  
    pthread_mutexattr_t attr;
    pthread_mutexattr_init ( &attr );
    pthread_mutexattr_settype ( &attr, PTHREAD_MUTEX_RECURSIVE );
    pthread_mutex_init ( &_lock, &attr );
#endif*/
}

spin_lock::~spin_lock( void ) {
	DeleteCriticalSection( &_lock );
	/*
#if defined(_WIN32) || defined(_WIN64)
    
#else 
    pthread_mutex_destroy(&_lock);
#endif
	*/

}

void spin_lock::lock( void ) {
	EnterCriticalSection( &_lock );
	/*
#if defined(_WIN32) || defined(_WIN64)
    
#else 
    pthread_mutex_lock(&_lock);
#endif
	*/
}

void spin_lock::unlock( void ){
	LeaveCriticalSection( &_lock );
	/*
#if defined(_WIN32) || defined(_WIN64)
    
#else 
    pthread_mutex_unlock(&_lock);
#endif
	*/
}

bool spin_lock::trylock( void ) {
	return TryEnterCriticalSection( &_lock ) == TRUE;
	/*
#if defined(_WIN32) || defined(_WIN64)
    
#else 
    return pthread_mutex_trylock(&_lock) == 0 ;
#endif
	*/
}
/*
#if defined(_WIN32) || defined(_WIN64)
#else
pthread_mutex_t& spin_lock::handle( void ) {
    return _lock;
}
#endif
*/

}}
