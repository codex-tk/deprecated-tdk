#ifndef __tdk_threading_thread_local_h__
#define __tdk_threading_thread_local_h__

#include <tdk/tdk.hpp>

namespace tdk { namespace threading {
/*
	thread local storage �� ����
	window �� ������ TlsAlloc , TlsFree , TlsSetValue , TlsGetValue ���� ���� �Ϸ�
	�̿��� �÷����� pthread �� �̿��� ������ ����
*/
class thread_local_impl {
public:
#if defined ( _WIN32 ) 
	typedef DWORD key_type;
#elif defined( linux ) || defined ( __linux )
	typedef pthread_key_t key_type;
#elif defined ( __MACOSX__ ) || defined ( __APPLE__ ) 

#else
	
#endif
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
	object type �� ������ thread local storage 
*/
template < typename object_type >
class thread_local_storage {
public:
	thread_local_storage( void ) {
		STATIC_CHECK( sizeof( object_type ) <= sizeof( void* ) , object_type_too_big );
	}
	~thread_local_storage( void ) {}
	
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
