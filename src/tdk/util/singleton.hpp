#ifndef __tdk_util_singleton_h__
#define __tdk_util_singleton_h__

#include <tdk/threading/atomic/atomic.hpp>
#include <tdk/threading/spin_lock.hpp>

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
		volatile type* barrier = instance_;
		tdk::threading::atomic_impl< 
			tdk::threading::atomic_trait< sizeof( void* ) >::value_type
		>::barrier();
		if( barrier == nullptr ) {
			tdk::threading::scoped_lock< lock_t > guard( lock_ );
			barrier = instance_;
			if( barrier == nullptr ) {
                barrier = creator_t<type>::create();
			    tdk::threading::atomic_impl< 
					tdk::threading::atomic_trait< sizeof( void* ) >::value_type
				>::barrier();
				instance_ = barrier;
			}
		}
		return const_cast< type* >( barrier );
	}
    
    static void __stdcall release( void ) {
        tdk::threading::scoped_lock< lock_t > guard( lock_ );
		volatile type* barrier = instance_;
		if( barrier != nullptr ) {
            creator_t<type>::release( const_cast< type* >( barrier ) );
            instance_ = nullptr;
		}
    }
private:
	static	volatile type* instance_;
	static	lock_t			lock_;
};

template < typename type , typename lock_t , template < typename > class creator_t >
volatile type* singleton< type , lock_t , creator_t >::instance_ = nullptr;

template < typename type , typename lock_t , template < typename > class creator_t >
lock_t singleton< type , lock_t , creator_t  >::lock_;


}}

#endif