#include <tdk/threading/atomic/atomic.hpp>

namespace tdk {
namespace threading {

void atomic32::barrier( void ) {
	__sync_synchronize();
}

atomic32::value_type atomic32::increment( atomic32::value_type* value ) {
	return __sync_add_and_fetch( value , 1 );
}

atomic32::value_type atomic32::decrement( atomic32::value_type* value ) {
	return __sync_sub_and_fetch( value , 1 ) ;
}


atomic32::value_type atomic32::exchange( atomic32::value_type* value , atomic32::value_type change ) {
	return 0;
}

atomic32::value_type atomic32::compare_and_swap( 
	  atomic32::value_type* value 
	, atomic32::value_type change
	, atomic32::value_type compare
) {
	return 0;
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
	__sync_synchronize();
}

atomic64::value_type atomic64::increment( atomic64::value_type* value ) {
	atomic64::value_type old;
	do {
		old = *value;
	} while ( compare_and_swap( value , old + 1 , old ) != old );
	return old + 1;
}

atomic64::value_type atomic64::decrement( atomic64::value_type* value ) {
	atomic64::value_type old;
	do {
		old = *value;
	} while ( compare_and_swap( value , old - 1 , old ) != old );
	return old - 1;
}

atomic64::value_type atomic64::exchange( atomic64::value_type* value , atomic64::value_type change ) {
	atomic64::value_type old;
	do {
		old = *value;
	} while ( compare_and_swap( value , change , old ) != old );
	return old;
}

atomic64::value_type atomic64::compare_and_swap( 
	  atomic64::value_type* value 
	, atomic64::value_type chg
	, atomic64::value_type cmp
) 
{
	return 0;
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
