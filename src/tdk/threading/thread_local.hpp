#ifndef __tdk_threading_thread_local_h__
#define __tdk_threading_thread_local_h__

#include <tdk/tdk.hpp>

namespace tdk { namespace threading {
/*
	thread local storage 의 구현
	window 의 구현은 TlsAlloc , TlsFree , TlsSetValue , TlsGetValue 으로 구현 완료
	이외의 플랫폼은 pthread 를 이용해 구현할 예정
*/
class thread_local_impl {
public:
	typedef int key_type;
	typedef void* value_type;
public:
	thread_local_impl( void );
	~thread_local_impl( void );

	void set( value_type value );
	value_type get( void );
private:
	key_type _key;
};

/*
	object type 을 가지는 thread local storage 
*/
template < typename object_type >
class thread_local {
public:
	thread_local( void ) {
		STATIC_CHECK( sizeof( object_type ) <= sizeof( void* ) , object_type_too_big );
	}
	~thread_local( void ) {}
	
	object_type get( void ) {	
		return reinterpret_cast< object_type >( _impl.get() );
	}

	void set( object_type value ) {
		_impl.set( reinterpret_cast< void* >( value ) );
	}
private:
	thread_local_impl _impl;
};

}}

#endif