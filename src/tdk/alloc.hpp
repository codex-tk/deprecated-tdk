#ifndef __tdk_alloc_h__
#define __tdk_alloc_h__

#include <tdk.hpp>

namespace tdk {

/*
	lib 내부에서 사용할 메모리 할당 관련
	hook 을 제공하여 어플리케이션 레벨에서 수정 가능하도록
	기본은 std::malloc 을 사용

	tc_malloc / je_malloc 붙여서 써보자
*/

void* alloc( std::size_t size );
void* realloc( void* p , std::size_t size );
void  free( void* p );

typedef void* (*alloc_handler)(std::size_t);
typedef void* (*realloc_handler)(void*, std::size_t);
typedef void  (*free_handler)(void*);

alloc_handler	 alloc_hook( alloc_handler function );
realloc_handler  realloc_hook( realloc_handler function );
free_handler	 free_hook( free_handler function );

class object_allocator {
public:
	//
    static void* operator new ( std::size_t sz ) {
		return tdk::alloc(sz);
	}
    static void  operator delete ( void* ptr ) {
		tdk::free(ptr);
	}
    // replacement new
    static void* operator new ( std::size_t sz , void* p) {
		return p;
	}
    static void  operator delete( void* , void* ) {
		//
	}
};

}
#endif
