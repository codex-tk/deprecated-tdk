#include "stdafx.h"
#include <tdk/threading/thread_local.hpp>
#include <cassert>
namespace tdk { namespace threading {

#if defined ( _WIN32 ) 
thread_local_impl::thread_local_impl( void ) 
{
	_key = TlsAlloc();
	if ( _key == TLS_OUT_OF_INDEXES ) {
		TDK_ASSERT( !"[tdk::threading::thread_local_impl::thread_local_impl()] TlsAlloc Failed" );
	}
}

thread_local_impl::~thread_local_impl( void ){
	if ( _key != TLS_OUT_OF_INDEXES )
		TlsFree( _key );
	_key = TLS_OUT_OF_INDEXES;
}

void thread_local_impl::set( thread_local_impl::value_type value ) {
	assert_msg( _key != TLS_OUT_OF_INDEXES 
		, "[tdk::threading::thread_local_impl::set()] TlsAlloc Failed" );
	if ( _key != TLS_OUT_OF_INDEXES )
		TlsSetValue( _key , value );
}

thread_local_impl::value_type thread_local_impl::get( void ) {
	assert_msg( _key != TLS_OUT_OF_INDEXES 
		, "[tdk::threading::thread_local_impl::get()] TlsAlloc Failed" );
	if ( _key != TLS_OUT_OF_INDEXES )
		return TlsGetValue( _key );
	return nullptr;
}

#elif defined( linux ) || defined ( __linux )
	
thread_local_impl::thread_local_impl( void ) 
{
	switch( pthread_key_create( &_key , nullptr ) ) {
	case EAGAIN:
		TDK_ASSERT( !"[tdk::threading::thread_local_impl::thread_local_impl()] pthread_key_create Failed" );
		break;
	}	
}

thread_local_impl::~thread_local_impl( void ){
	pthread_key_delete( _key );
}

void thread_local_impl::set( thread_local_impl::value_type value ) {
	pthread_setspecific( _key , value );
}

thread_local_impl::value_type thread_local_impl::get( void ) {
	return pthread_getspecific( _key );
}

#elif defined ( __MACOSX__ ) || defined ( __APPLE__ ) 

#else
	
#endif
	

}}
