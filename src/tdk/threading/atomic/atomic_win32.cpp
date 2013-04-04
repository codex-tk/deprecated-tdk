#include "stdafx.h"
#include <tdk/threading/atomic/atomic.hpp>

namespace tdk {
namespace threading {

void atomic32::barrier( void ) {
	::MemoryBarrier();
}

#define __ATOMIC_ASM__	
atomic32::value_type atomic32::increment( atomic32::value_type* value ) {
#if defined( __ATOMIC_ASM__ ) && !defined ( _M_X64 )
	/*
	XADD r/m32, r32	 Exchange r32 and r/m32; load sum into r/m32.
	*/
	atomic32::value_type result = 0;
	_asm { 
			mov  ecx , value 
			mov  eax , 1
	   lock xadd dword ptr [ecx] , eax		
			//mov  result , eax
			inc eax
	}
	//return result + 1;
#else 
	return ::InterlockedIncrement( reinterpret_cast< volatile LONG* >( value ) );
#endif
}

atomic32::value_type atomic32::decrement( atomic32::value_type* value ) {
#if defined( __ATOMIC_ASM__ ) && !defined ( _M_X64 )
	/*
	XADD r/m32, r32	 Exchange r32 and r/m32; load sum into r/m32.
	*/
	atomic32::value_type result = 0;
	_asm { 
			mov  ecx , value 
			mov  eax , -1
	   lock xadd dword ptr [ecx] , eax		
		//	mov  result , eax
			dec eax
	}
	//return result - 1;
#else 
	return ::InterlockedDecrement( reinterpret_cast< volatile LONG* >( value ) );
#endif
}


atomic32::value_type atomic32::exchange( atomic32::value_type* value , atomic32::value_type change ) {
#if defined( __ATOMIC_ASM__ ) && !defined ( _M_X64 )
	atomic32::value_type result = 0;
	_asm { 
			mov  ecx , value 
			mov  eax , change
	   lock xchg dword ptr [ecx] , eax		
	   // eax 에는 *value 의 예전 값
	}
#else 
	return ::InterlockedExchange( reinterpret_cast< volatile LONG* >( value ) , change );
#endif
}

atomic32::value_type atomic32::compare_and_swap( 
	  atomic32::value_type* value 
	, atomic32::value_type change
	, atomic32::value_type compare
) {
#if defined( __ATOMIC_ASM__ ) && !defined ( _M_X64 )
	atomic32::value_type result = 0;
	_asm { 
			mov  ecx , value 
			mov	 edx , change
			mov  eax , compare
	   lock cmpxchg dword ptr [ecx] , edx	
	}
#else 
	return ::InterlockedCompareExchange( reinterpret_cast< volatile LONG* >( value ) , change , compare );
#endif
}


bool		  atomic32::compare_and_swap_bool(  value_type* value , value_type change , value_type compare ){
	return compare_and_swap( value , change , compare ) == compare;
}

atomic32::value_type		  atomic32::add(  value_type* value , value_type add  ){
	atomic32::value_type old;
	do {
		old = *value;
	} while ( compare_and_swap( value , old + add , old ) != old );
	return old + add;
}

void atomic64::barrier( void ) {
	::MemoryBarrier();
}

atomic64::value_type atomic64::increment( atomic64::value_type* value ) {
#if defined( _M_X64 ) 
	return InterlockedIncrement64( reinterpret_cast< volatile LONGLONG* >( value ) );
#else 
	atomic64::value_type old;
	do {
		old = *value;
	} while ( compare_and_swap( value , old + 1 , old ) != old );
	return old + 1;
#endif
}

atomic64::value_type atomic64::decrement( atomic64::value_type* value ) {
#if defined( _M_X64 )
	return InterlockedDecrement64( reinterpret_cast< volatile LONGLONG* >( value ) );
#else 
	atomic64::value_type old;
	do {
		old = *value;
	} while ( compare_and_swap( value , old - 1 , old ) != old );
	return old - 1;
#endif
}

atomic64::value_type atomic64::exchange( atomic64::value_type* value , atomic64::value_type change ) {
#if defined( _M_X64 )
	return ::InterlockedExchange64( reinterpret_cast< volatile LONGLONG* >( value ) , change );
#else
	atomic64::value_type old;
	do {
		old = *value;
	} while ( compare_and_swap( value , change , old ) != old );
	return old;
#endif
}

atomic64::value_type atomic64::compare_and_swap( 
	  atomic64::value_type* value 
	, atomic64::value_type chg
	, atomic64::value_type cmp
) 
{
#if defined( _M_X64 )
	return InterlockedCompareExchange64( value , chg , cmp );
#else 
	atomic64::value_type result		= 0;
	atomic64::value_type change		= chg;
	atomic64::value_type compare	= cmp;
	_asm {
			push ebx
			push edi

			mov ecx , dword ptr change + 4 
			mov ebx , dword ptr change
			mov edx , dword ptr compare + 4
			mov eax , dword ptr compare
			mov edi , value
			lock cmpxchg8b qword ptr [edi]

			mov dword ptr result + 4 , edx
			mov dword ptr result , eax

			pop edi
			pop ebx
	}
	return result;
#endif
}


bool		  atomic64::compare_and_swap_bool(  value_type* value , value_type change , value_type compare ){
	return compare_and_swap( value , change , compare ) == compare;
}


atomic64::value_type		  atomic64::add(  value_type* value , value_type add  ){
	atomic64::value_type old;
	do {
		old = *value;
	} while ( compare_and_swap( value , old + add , old ) != old );
	return old + add;
}

}}