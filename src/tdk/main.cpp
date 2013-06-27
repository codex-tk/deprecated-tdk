#include <tdk/threading/atomic/atomic.hpp>
//#include <stdint.h>
#include <cstdio>

int main( int argc , char* argv[] ){
    tdk::threading::atomic<int> val;
	printf( "%d" , val.load());
	printf( "%d" , val.increment());

	val.barrier();

	tdk::threading::atomic32::value_type v = 0;

	tdk::threading::atomic32::increment( &v );
	

	volatile int32_t value = 0;

	int val1 = __sync_add_and_fetch( &value , 1 );

	printf( "%d" , val1 );

	return 0;
}
