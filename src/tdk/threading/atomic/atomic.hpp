#ifndef __tck_threading_atomic_h__
#define __tck_threading_atomic_h__

#include <tdk/tdk.hpp>
#include <tdk/threading/atomic/memory_order.hpp>

namespace tdk {
namespace threading {

/*
	acquire / release sementics

	An operation has acquire semantics if other processors will always see its effect before any subsequent operation's effect. 
	An operation has release semantics if other processors will see every preceding operation's effect before the effect of the operation itself.

	acquire - guarantees that memory reads can't jump over the barrier

	InterlockedIncrementAcquire(&a);
	b++;
	c++;

	other processors would always see the increment of a before the increments of b and c.

	release - guarantees that memory writes can't jump over the barrier

	a++;
	b++;
	InterlockedIncrementRelease(&c);

	other processors would always see the increments of a and b before the increment of c.

*/

/*
	atomic operation
	윈도우는 assembly 나 Interlocked 함수로 구현
	linux 는 gcc 함수로 처리 예정
	ios 는 OSAtomic 으로 처리 예정 <- cas 가 업고 tas 이 있음.
*/


	
class atomic32 {
public:
	typedef volatile int32_t value_type;
	static void		  __stdcall barrier( void );
	static value_type __stdcall increment( value_type* value );
	static value_type __stdcall decrement( value_type* value );
	static value_type __stdcall exchange( value_type* value , value_type change );
	static value_type __stdcall compare_and_swap( value_type* value , value_type change , value_type compare );
	static bool		  __stdcall compare_and_swap_bool(  value_type* value , value_type change , value_type compare );
	static value_type __stdcall add( value_type* value , value_type add );
};

/*
	lock free container 용도로 작성.
	현재는 윈도우 전용
	assembly 로 구현
	cmpxchg8b 로 구현한 cas 함수 베이스로 작동됨
	이외의 플랫폼은 차후 생각
*/

class atomic64 {
public:	
	typedef volatile int64_t value_type;
	static void		  __stdcall barrier( void );
	static value_type __stdcall increment( value_type* value );
	static value_type __stdcall decrement( value_type* value );
	static value_type __stdcall exchange( value_type* value , value_type change );
	static value_type __stdcall compare_and_swap( value_type* value , value_type change , value_type compare );
	static bool		  __stdcall compare_and_swap_bool(  value_type* value , value_type change , value_type compare );
	static value_type __stdcall add( value_type* value , value_type add );
};

template < typename T_type > 
struct atomic_impl;

template<> struct atomic_impl< atomic32::value_type > : public atomic32 {};
template<> struct atomic_impl< atomic64::value_type > : public atomic64 {};

template < int >
struct atomic_trait {
	typedef atomic32::value_type value_type;
};

template <> struct atomic_trait<5> {
	typedef atomic64::value_type value_type;
};

template <> struct atomic_trait<6> {
	typedef atomic64::value_type value_type;
};

template <> struct atomic_trait<7> {
	typedef atomic64::value_type value_type;
};

template <> struct atomic_trait<8> {
	typedef atomic64::value_type value_type;
};

template < typename T >
struct remove_volatile {
	typedef T type;
};

template < typename T >
struct remove_volatile< volatile T  >{
	typedef T type;
};

template < typename T_object >
class atomic {
public:
	typedef typename atomic_trait< sizeof(T_object) >::value_type value_type;

	atomic( void ) : _value( 0 ){
		STATIC_CHECK( sizeof( T_object ) <= sizeof( int64_t ) , atomic_type_too_big );
	}

	explicit atomic( const T_object& val ): _value(val) {
		STATIC_CHECK( sizeof( T_object ) <= sizeof( int64_t ) , atomic_type_too_big );
	}

	T_object load( void ) const {
		return *static_cast< value_type* >( const_cast< value_type* >(&_value ));
	}

	template < typename T_memory_order >
	T_object load( T_memory_order& ord ) {
		auto_barrier< T_memory_order > barrier( ord );
		return load();
	}

	void store( const T_object& obj ) {
		_value = obj;
	}

	template < typename T_memory_order >
	void store( const T_object& obj , T_memory_order& ord ) {
		auto_barrier< T_memory_order > barrier( ord );
		store( obj );
	}
	
	T_object increment( void ) {
		return atomic_impl< value_type >::increment( &_value );
	}

	template < typename T_memory_order >
	T_object increment( T_memory_order& ord ) {
		auto_barrier< T_memory_order > barrier( ord );
		return increment();
	}

	T_object decrement( void ) {
		return atomic_impl< value_type >::decrement( &_value );
	}

	template < typename T_memory_order >
	T_object decrement( T_memory_order& ord ) {
		auto_barrier< T_memory_order > barrier( ord );
		return decrement();
	}

	T_object exchange( T_object change ) {
		return atomic_impl< value_type >::exchange( &_value , change );
	}

	T_object compare_and_swap( T_object change , T_object compare ) {
		return atomic_impl< value_type >::compare_and_swap( &_value , change , compare );
	}

	T_object add( T_object a ) {
		return atomic_impl< value_type >::add( &_value , a );
	}

	T_object or( T_object a ) {
		T_object old;
		do {
			old = load();
		} while ( compare_and_swap( old | a , old ) != old );
		return old;
	}

	T_object and( T_object a ) {
		T_object old;
		do {
			old = load();
		} while ( compare_and_swap( old & a , old ) != old );
		return old;
	}
	

	template < typename T_memory_order >
	T_object exchange( T_object change , T_memory_order& ord ) {
		auto_barrier< T_memory_order > barrier( ord );
		return exchange( change );
	}

	template < typename T_memory_order >
	T_object compare_and_swap( T_object change , T_object compare , T_memory_order& ord ) {
		auto_barrier< T_memory_order > barrier( ord );
		return compare_and_swap( change , compare );
	}

	template < typename T_memory_order >
	T_object add( T_object a , T_memory_order& ord ) {
		auto_barrier< T_memory_order > barrier( ord );
		return add( a );
	}

#if defined ( _MSC_VER )
	__declspec(align(8)) value_type _value;
#else
	value_type _value __attribute__((aligned(8)));
#endif
};

}
}

#endif