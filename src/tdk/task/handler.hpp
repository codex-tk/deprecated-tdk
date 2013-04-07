#ifndef __tdk_task_handler_h__
#define __tdk_task_handler_h__

#include "tdk.hpp"
#include <functional>

namespace tdk {
namespace task {

namespace {
template < typename T >
struct forward_type {
    typedef const T& type;
};

template < typename T >
struct forward_type < const T > {
    typedef const T& type;
};

template < typename T >
struct forward_type < T& > {
    typedef T& type;
};

template < typename T >
struct forward_type < const T& > {
    typedef const T& type;
};

template < typename T >
struct forward_type < T* > {
    typedef T* type;
};

template < typename T >
struct forward_type < const T* > {
    typedef const T* type;
};

template <>
struct forward_type< int > {
    typedef int type;
};

template <>
struct forward_type< const int > {
    typedef const int type;
};

template < typename T >
struct param_trait {
	typedef typename forward_type<T>::type type;
};

struct null_type{
	explicit null_type(){}
};

template < typename T >
struct type2type {
	typedef T type;
};

template < typename T >
struct reference_wrapper {
    explicit reference_wrapper( T& t ) : t_( &t ) {}
    operator T& () const { return *t_; }
private:
    T* t_;
};

template < typename T >
static reference_wrapper< T > ref( T& t ) {
    return reference_wrapper< T > ( t );
}

template < typename T >
static reference_wrapper< T const > cref( T& t ) {
    return reference_wrapper< T const > ( t );
}


#define TEMPLATE_DECLARE_0  typename R
#define TEMPLATE_DECLARE_1  TEMPLATE_DECLARE_0 , typename P1
#define TEMPLATE_DECLARE_2  TEMPLATE_DECLARE_1 , typename P2
#define TEMPLATE_DECLARE_3  TEMPLATE_DECLARE_2 , typename P3
#define TEMPLATE_DECLARE_4  TEMPLATE_DECLARE_3 , typename P4
#define TEMPLATE_DECLARE_5  TEMPLATE_DECLARE_4 , typename P5
#define TEMPLATE_DECLARE_6  TEMPLATE_DECLARE_5 , typename P6

#define TEMPLATE_SP_DECLARE_0  
#define TEMPLATE_SP_DECLARE_1 P1
#define TEMPLATE_SP_DECLARE_2 TEMPLATE_SP_DECLARE_1 , P2
#define TEMPLATE_SP_DECLARE_3 TEMPLATE_SP_DECLARE_2 , P3
#define TEMPLATE_SP_DECLARE_4 TEMPLATE_SP_DECLARE_3 , P4
#define TEMPLATE_SP_DECLARE_5 TEMPLATE_SP_DECLARE_4 , P5
#define TEMPLATE_SP_DECLARE_6 TEMPLATE_SP_DECLARE_5 , P6

#define TEMPLATE_SP_0 R ( TEMPLATE_SP_DECLARE_0 )
#define TEMPLATE_SP_1 R ( TEMPLATE_SP_DECLARE_1 )
#define TEMPLATE_SP_2 R ( TEMPLATE_SP_DECLARE_2 )
#define TEMPLATE_SP_3 R ( TEMPLATE_SP_DECLARE_3 )
#define TEMPLATE_SP_4 R ( TEMPLATE_SP_DECLARE_4 )
#define TEMPLATE_SP_5 R ( TEMPLATE_SP_DECLARE_5 )
#define TEMPLATE_SP_6 R ( TEMPLATE_SP_DECLARE_6 )

#define TEMPLATE_PARAM_0 
#define TEMPLATE_PARAM_1 typename param_trait< P1 >::type p1
#define TEMPLATE_PARAM_2 TEMPLATE_PARAM_1 , typename param_trait< P2 >::type p2
#define TEMPLATE_PARAM_3 TEMPLATE_PARAM_2 , typename param_trait< P3 >::type p3
#define TEMPLATE_PARAM_4 TEMPLATE_PARAM_3 , typename param_trait< P4 >::type p4
#define TEMPLATE_PARAM_5 TEMPLATE_PARAM_4 , typename param_trait< P5 >::type p5
#define TEMPLATE_PARAM_6 TEMPLATE_PARAM_5 , typename param_trait< P6 >::type p6

#define TEMPLATE_PARAM_FORWARD_0 
#define TEMPLATE_PARAM_FORWARD_1 p1 
#define TEMPLATE_PARAM_FORWARD_2 TEMPLATE_PARAM_FORWARD_1 , p2
#define TEMPLATE_PARAM_FORWARD_3 TEMPLATE_PARAM_FORWARD_2 , p3
#define TEMPLATE_PARAM_FORWARD_4 TEMPLATE_PARAM_FORWARD_3 , p4
#define TEMPLATE_PARAM_FORWARD_5 TEMPLATE_PARAM_FORWARD_4 , p5
#define TEMPLATE_PARAM_FORWARD_6 TEMPLATE_PARAM_FORWARD_5 , p6



template < typename F >
class call_wrapper;

#define DECLARE_CALLWRAPPER( TEMPLATE_DECLARE , TEMPLATE_SP , TEMPLATE_PARAM , TEMPLATE_PARAM_FORWARD )\
template < TEMPLATE_DECLARE >\
class call_wrapper< TEMPLATE_SP > {\
public:\
    call_wrapper( void ){}\
    virtual ~call_wrapper( void ){}\
    virtual R operator()( TEMPLATE_PARAM ) const = 0;\
    virtual call_wrapper* copy( void* ptr , int size ) const = 0;\
public:\
    template < typename function_t >\
    class function_call_wrapper : public call_wrapper {\
    public:\
        function_call_wrapper( typename param_trait<function_t>::type f ) \
            : _function(f) {}\
        virtual ~function_call_wrapper( void ) {} \
        virtual R operator()( TEMPLATE_PARAM ) const {\
            return _function( TEMPLATE_PARAM_FORWARD );\
        }\
        virtual call_wrapper* copy( void* ptr , int size ) const {\
			if ( ( ptr == nullptr ) || ( sizeof( *this ) > size) ) {\
				ptr = std::malloc( sizeof(*this));\
			}\
            new ( ptr ) function_call_wrapper(_function);\
            return static_cast< call_wrapper* >( ptr );\
        } \
    private:\
        function_t _function;\
    };\
    template < typename function_t , typename object_t > \
    class member_function_call_wrapper : public call_wrapper {\
    public:\
        member_function_call_wrapper( const function_t& f , const object_t& obj ) \
            : _function(f) , _Object(obj){}\
        virtual ~member_function_call_wrapper( void ) {} \
        virtual R operator()( TEMPLATE_PARAM ) const {\
            return ((*_Object).*_function)( TEMPLATE_PARAM_FORWARD );\
        }\
        virtual call_wrapper* copy( void* ptr , int size ) const {\
			if ( ( ptr == nullptr ) || ( sizeof( *this ) > size) ) {\
				ptr = std::malloc( sizeof(*this));\
			}\
            new ( ptr ) member_function_call_wrapper( _function , _Object );\
            return static_cast< call_wrapper* >( ptr );\
        } \
    private:\
        object_t   _Object;\
        function_t _function;\
    };\
    template < typename F >\
    static call_wrapper* __stdcall bind( const F& f  ) {\
		void* ptr = std::malloc( sizeof( function_call_wrapper<F> ));\
        new ( ptr ) function_call_wrapper<F>(f);\
        return static_cast< call_wrapper* >( ptr );\
    }\
    template < typename F >\
    static call_wrapper* __stdcall bind( const F& f , void* ptr , int bufSize ) {\
		if ( ( ptr == nullptr ) || ( sizeof( function_call_wrapper<F> ) > bufSize)) {\
			ptr = std::malloc( sizeof( function_call_wrapper<F> ));\
		}\
        new ( ptr ) function_call_wrapper<F>(f);\
        return static_cast< call_wrapper* >( ptr );\
    }\
    template < typename F , typename T >\
    static call_wrapper* __stdcall bind( const F& f , const T& t ) {\
		void* ptr = std::malloc( sizeof( member_function_call_wrapper<F , T>));\
        new ( ptr ) member_function_call_wrapper<F , T>(f,t);\
        return static_cast< call_wrapper* >( ptr );\
    }\
    template < typename F , typename T >\
    static call_wrapper* __stdcall bind( const F& f ,  const T& t ,void* ptr , int bufSize ) {\
		if ( ( ptr == nullptr ) || ( sizeof( member_function_call_wrapper<F,T> ) > bufSize)) {\
			ptr = std::malloc( sizeof( member_function_call_wrapper<F,T> ));\
		}\
        new ( ptr ) member_function_call_wrapper<F,T>(f,t);\
        return static_cast< call_wrapper* >( ptr );\
    }\
};


#define DECLARE_CALLWRAPPER_PARAM_N( n )\
DECLARE_CALLWRAPPER( TEMPLATE_DECLARE_##n ,\
                     TEMPLATE_SP_##n      ,\
                     TEMPLATE_PARAM_##n   ,\
                     TEMPLATE_PARAM_FORWARD_##n )

DECLARE_CALLWRAPPER_PARAM_N(0)
DECLARE_CALLWRAPPER_PARAM_N(1)
DECLARE_CALLWRAPPER_PARAM_N(2)
DECLARE_CALLWRAPPER_PARAM_N(3)
DECLARE_CALLWRAPPER_PARAM_N(4)
DECLARE_CALLWRAPPER_PARAM_N(5)
DECLARE_CALLWRAPPER_PARAM_N(6)

}

template < typename T >
class handler;

#define DECLARE_CALLBACK_HANDLER( TEMPLATE_DECLARE , TEMPLATE_SP , TEMPLATE_PARAM , TEMPLATE_PARAM_FORWARD )\
template < TEMPLATE_DECLARE > \
class handler< TEMPLATE_SP > { \
private:\
    enum { LOCAL_BUFFER_SIZE = sizeof(void*)*2 };\
    typedef call_wrapper< TEMPLATE_SP > call_wrapper; \
public: \
    handler( void ) : _call_wrapper( nullptr ){ } \
    \
	template < typename function_t >\
	explicit handler( const function_t& func ) \
		: _call_wrapper( call_wrapper::bind( func , _local_buffer , LOCAL_BUFFER_SIZE ) ){}\
	\
	template < typename function_t , typename object_t >\
	explicit handler( const function_t& func , const object_t& object )\
        : _call_wrapper( call_wrapper::bind( func , object , _local_buffer , LOCAL_BUFFER_SIZE ) ) {}\
    \
	handler( const handler& rhs ) : _call_wrapper( rhs.copy( _local_buffer , LOCAL_BUFFER_SIZE ) ) {}\
    \
	handler& operator=( const handler& rhs ) { \
		handler h( rhs );\
		clean_up();\
		_call_wrapper =  h.copy( _local_buffer , LOCAL_BUFFER_SIZE );\
		return *this;\
	}\
    \
	template < typename function_t >\
	handler& operator=( const function_t& func ) {\
		clean_up();\
		_call_wrapper = call_wrapper::bind( func , _local_buffer , LOCAL_BUFFER_SIZE );\
		return *this;\
	}\
    \
	~handler( void ) {\
		clean_up();\
	}\
    \
	void clean_up() {\
		if ( _call_wrapper != nullptr ) { \
			if ( _is_allocated() ) {\
				_call_wrapper->~call_wrapper();\
				std::free( _call_wrapper );\
			} else \
				_call_wrapper->~call_wrapper();\
			_call_wrapper = nullptr;\
		}\
	}\
    \
	R operator()( TEMPLATE_PARAM )const {\
		TDK_ASSERT( _call_wrapper != nullptr && "" );\
		return (*_call_wrapper)( TEMPLATE_PARAM_FORWARD );\
	}\
    bool is_good(void)const {\
        return _call_wrapper != nullptr;\
    }\
private:   \
	bool _is_allocated( void ) const {\
		return _call_wrapper != reinterpret_cast< call_wrapper* >( const_cast< uint8_t* >( _local_buffer ) );\
	}\
	\
	call_wrapper* copy( void* p , int size ) const {\
		if ( _call_wrapper != nullptr ) {\
			return _call_wrapper->copy( p , size );\
		}\
		return nullptr;\
	}\
private:\
	uint8_t			_local_buffer[ LOCAL_BUFFER_SIZE ];\
	call_wrapper*	_call_wrapper;\
};\


#define DECLARE_CALLBACK_HANDLER_PARAM_N( n )\
DECLARE_CALLBACK_HANDLER( TEMPLATE_DECLARE_##n ,\
                          TEMPLATE_SP_##n      ,\
                          TEMPLATE_PARAM_##n   ,\
                          TEMPLATE_PARAM_FORWARD_##n )

DECLARE_CALLBACK_HANDLER_PARAM_N(0)
DECLARE_CALLBACK_HANDLER_PARAM_N(1)
DECLARE_CALLBACK_HANDLER_PARAM_N(2)
DECLARE_CALLBACK_HANDLER_PARAM_N(3)
DECLARE_CALLBACK_HANDLER_PARAM_N(4)
DECLARE_CALLBACK_HANDLER_PARAM_N(5)
DECLARE_CALLBACK_HANDLER_PARAM_N(6)



}}

#endif