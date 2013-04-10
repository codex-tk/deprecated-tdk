#ifndef __tck_threading_memory_order_h__
#define __tck_threading_memory_order_h__

#include <tdk/tdk.hpp>

namespace tdk {
namespace threading {

struct acquire {
	void acquire_barrier( void );
};

struct no_acquire {
	void acquire_barrier( void );
};

struct release {
	void release_barrier( void );
};

struct no_release {
	void release_barrier( void );
};

template < typename T_acquire , typename T_release > 
class memory_order 
	: public T_acquire
	, public T_release
{

};

static memory_order< acquire	, no_release	> memory_order_acquire;
static memory_order< no_acquire , no_release	> memory_order_no_barrier;
static memory_order< no_acquire	, release		> memory_order_release;
static memory_order< acquire	, release		> memory_order_barrier;

template < typename T_order >
struct auto_barrier {
public:
	auto_barrier( T_order& ord ) 
		: _order( ord ) {
		_order.release_barrier();
	}

	~auto_barrier( void ) {
		_order.acquire_barrier();
	}

private:
	T_order& _order;
};

}}

#endif