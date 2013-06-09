#include "stdafx.h"
#include <tdk/threading/thread_local.hpp>

namespace tdk { namespace threading {

thread_local_impl::thread_local_impl( void ) 
	: _key( TlsAlloc() )
{
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

}}