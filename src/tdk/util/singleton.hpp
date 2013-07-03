#ifndef __tdk_util_singleton_h__
#define __tdk_util_singleton_h__

#include <tdk/threading/spin_lock.hpp>
#include <atomic>

namespace tdk {
namespace util {
template < typename type >
struct default_creator {
	static type* create( void  ) { 
		return new type();	
	}

	static void release( type* t ) {
		delete t;			
	}
	
	static type* create( const type& t ) {
		return new type( t ); 
	}
};

template < 
    typename type ,	
	typename lock_t = tdk::threading::spin_lock,	
	template < typename > class creator_t = default_creator
>
class singleton{
public:
	static type* __stdcall instance( void  ) {
		type* barrier = _instance.load();
		if( barrier == nullptr ) {
			tdk::threading::scoped_lock< lock_t > guard( _lock );
			barrier = _instance.load();
			if( barrier == nullptr ) {
                barrier = creator_t<type>::create();
				_instance.store( barrier );
			}
		}
		return const_cast< type* >( barrier );
	}
    
    static void __stdcall release( void ) {
        tdk::threading::scoped_lock< lock_t > guard( _lock );
		type* barrier = _instance;
		if( barrier != nullptr ) {
            creator_t<type>::release( const_cast< type* >( barrier ) );
            _instance.store( nullptr );
		}
    }
private:
	static std::atomic< type* > _instance;
	static lock_t _lock;
};

template < typename type , typename lock_t , template < typename > class creator_t >
std::atomic< type* > singleton< type , lock_t , creator_t >::_instance;

template < typename type , typename lock_t , template < typename > class creator_t >
lock_t singleton< type , lock_t , creator_t  >::_lock;


}}

#endif