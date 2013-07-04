#include "stdafx.h"
#include <tdk/threading/spin_lock.hpp>

namespace tdk {
namespace threading {

#if defined ( _WIN32 ) 

spin_lock::spin_lock( void ) {
    BOOL ret = InitializeCriticalSectionAndSpinCount( &_lock , 4000 );
}

spin_lock::~spin_lock( void ) {
    DeleteCriticalSection( &_lock );
}

void spin_lock::lock( void ) {
	EnterCriticalSection( &_lock );
}

void spin_lock::unlock( void ){
	LeaveCriticalSection( &_lock );
}

bool spin_lock::trylock( void ) {
    return TryEnterCriticalSection( &_lock ) == TRUE;
}

#elif defined( linux ) || defined ( __linux )

spin_lock::spin_lock( void ) {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init ( &attr );
    pthread_mutexattr_settype ( &attr, PTHREAD_MUTEX_RECURSIVE );
    pthread_mutex_init ( &_lock, &attr );
}

spin_lock::~spin_lock( void ) {
    pthread_mutex_destroy(&_lock);
}

void spin_lock::lock( void ) {
	pthread_mutex_lock(&_lock);
}

void spin_lock::unlock( void ){
	pthread_mutex_unlock(&_lock);
}

bool spin_lock::trylock( void ) {
    return pthread_mutex_trylock(&_lock) == 0 ;
}

pthread_mutex_t& spin_lock::handle( void ) {
    return _lock;
}

#elif defined ( __MACOSX__ ) || defined ( __APPLE__ ) 

#else

#endif
	
}}
