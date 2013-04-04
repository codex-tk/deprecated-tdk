#ifndef __tdk_spin_lock_h__
#define __tdk_spin_lock_h__

#include "tdk.hpp"

namespace tdk {
namespace threading {

class spin_lock {
public:
	spin_lock( void );
	~spin_lock( void );

	void lock( void );
	void unlock( void );
	bool trylock( void );
#if defined(_WIN32) || defined(_WIN64)
#else 
    pthread_mutex_t& handle( void );
#endif
private:
#if defined(_WIN32) || defined(_WIN64)
    CRITICAL_SECTION _lock;
#else 
    pthread_mutex_t _lock;
#endif
};

}}

//#include "scoped_lock.hpp"

#endif