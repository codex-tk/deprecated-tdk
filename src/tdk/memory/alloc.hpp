#ifndef __tdk_memory_alloc_h__
#define __tdk_memory_alloc_h__

/*
	현재는 미사용

	개념은 파악했으니

	tc_malloc 이나 je malloc 쓰고 다른것에 집중하자
	

	todo 
	1. statisics 관련 처리 
	 - 전처리기로 처리 할것
	 - 할당 횟수 , 반납 횟수 , 재할당률
	2. 이벤트 기반 메모리 반납 처리 
	3. last error 처리
	 - over flow
	 - corrupted
	 - dup free
*/
namespace tdk { namespace memory { 

typedef void* (*alloc_hook_fn)(std::size_t);
typedef void* (*realloc_hook_fn)(void*, std::size_t);
typedef void  (*free_hook_fn)(void*);

void* alloc( std::size_t size );
void* realloc( void* p , std::size_t size );
void  free( void* p );

void  alloc_hook( alloc_hook_fn function );
void  realloc_hook( realloc_hook_fn function );
void  free_hook( free_hook_fn function );

alloc_hook_fn alloc_hook( void );
realloc_hook_fn realloc_hook( void );
free_hook_fn free_hook( void );

struct malloc_allocator {
	static void* alloc( std::size_t size );
	static void* realloc( void* p , std::size_t size );
	static void  free( void* p );
};

struct lib_allocator {
	static void* alloc( std::size_t size );
	static void* realloc( void* p , std::size_t size );
	static void  free( void* p );
};

struct tc_allocator {
	static void* alloc( std::size_t size );
	static void* realloc( void* p , std::size_t size );
	static void  free( void* p );
};

}}

#endif