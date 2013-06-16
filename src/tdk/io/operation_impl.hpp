#ifndef __tdk_io_operation_impl_h__
#define __tdk_io_operation_impl_h__

namespace tdk {
namespace io {
namespace detail {

#ifndef FORWARD_HANDLER_0
#define FORWARD_HANDLER_0( handler ) void operator()() const { handler(); }
#endif

#ifndef FORWARD_HANDLER_1
#define FORWARD_HANDLER_1( handler ) FORWARD_HANDLER_0(handler) template < typename A1 > void operator()(A1& a1) const { handler(a1);}
#endif

#ifndef FORWARD_HANDLER_2
#define FORWARD_HANDLER_2( handler ) FORWARD_HANDLER_1(handler) template < typename A1 , typename A2 > void operator()( A1& a1 , A2& a2 ) const { handler( a1 , a2 );}
#endif


template < typename T_handler
	, typename T_executor >
struct wrapped_handler {
	T_handler  handler;
	T_executor* executor;

	wrapped_handler( const T_handler& h , T_executor* e )
		: handler (h) , executor(e){
	}

	wrapped_handler( const wrapped_handler& rhs ) 
		: handler( rhs.handler )
		, executor(rhs.executor){
	}
	FORWARD_HANDLER_2( handler );
};

/*

template < typename T_handler >
struct cancelable_handler {
	T_handler handler;
};

*/

template < typename T_handler >
class execution_plan {
public:
	template < typename T_dispatch , typename T_delete >
	static void select( operation* op 
		, const T_handler& h 
		, const T_dispatch& do_dispatch 
		, const T_delete& do_delete )
	{
		do_dispatch( op );
		do_delete( op );
	}
};

template < typename T_handler , typename T_executor >
class execution_plan< wrapped_handler< T_handler , T_executor > > {
public:
	template < typename T_dispatch , typename T_delete >
	static void select( operation* op 
		, const wrapped_handler< T_handler , T_executor >& h 
		, const T_dispatch& do_dispatch 
		, const T_delete& do_delete )
	{
		h.executor->execute( op , do_dispatch , do_delete );
	}
};

template < typename T_base >
class dispatcher_error_code {
public:
	template < typename T_handler >
	static void dispatch( const T_handler& h , T_base* op ) {
		h( op->error());
	}
};


template < typename T_base >
class dispatcher_error_code_and_io_bytes {
public:
	template < typename T_handler >
	static void dispatch( const T_handler& h , T_base* op ) {
		h( op->error() , op->io_bytes());
	}
};

template < typename T_op >
class release_policy_delete {
public:
	static void release( T_op* op ) {
		delete op;
	}
};

template < typename T_op >
class release_policy_release {
	static void release( T_op* op ) {
		op->release();
	}
};



template < typename T_handler , typename T_base 
	, template < typename base > class T_dispatcher  
	, template < typename op > class T_release_policy = release_policy_delete >
class operation_impl : public T_base {
public:
	/*
	template < typename... Args >
	operation_impl( const T_handler& h , Args... args ) 
		: _handler( h ) 
		, T_base(&operation_impl::_on_complete , args ) 
	{
	}
	*/

	operation_impl( const T_handler& h )
		: T_base( &operation_impl::_on_complete )
		, _handler(h) 
	{
	}

	template < typename A1 >
	operation_impl( const T_handler& h , A1& a1 )
		: T_base( &operation_impl::_on_complete , a1 )
		, _handler(h) 
	{
	}

	template < typename A1 , typename A2 >
	operation_impl( const T_handler& h , A1& a1 , A2& a2 )
		: T_base( &operation_impl::_on_complete , a1 , a2 )
		, _handler(h)
	{
	}

	template < typename A1 , typename A2 ,  typename A3 >
	operation_impl( const T_handler& h , A1& a1 , A2& a2 , A3& a3 )
		: T_base( &operation_impl::_on_complete , a1 , a2 , a3 )
		, _handler(h)
	{
	}

	~operation_impl(void){
	}

	void on_complete( void ){
		if ( end_opearation() ) {
			execution_plan< T_handler >::select( this 
				, _handler 
				, &operation_impl::_do_dispatch
				, &operation_impl::_do_delete );			 
		}
	}

	void do_dispatch( void ){
		T_dispatcher<T_base>::dispatch( _handler , this );
	}

	static void __stdcall _do_dispatch( operation* op ) {
		operation_impl* impl( static_cast< operation_impl* >(op));
		impl->do_dispatch();
	}

	static void __stdcall _do_delete( operation* op ) {
		operation_impl* impl( static_cast< operation_impl* >(op));
		T_release_policy< operation_impl >::release( impl ); 
	}

	static void __stdcall _on_complete( operation* op ) {
		operation_impl* impl( static_cast< operation_impl* >(op));
  		impl->on_complete();
  	}
private:
	T_handler _handler;
};

}}}


#endif