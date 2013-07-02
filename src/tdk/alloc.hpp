#ifndef __tdk_alloc_h__
#define __tdk_alloc_h__

namespace tdk {

typedef void* (*alloc_handler)(std::size_t);
typedef void* (*realloc_handler)(void*, std::size_t);
typedef void  (*free_handler)(void*);

void* alloc( std::size_t size );
void* realloc( void* p , std::size_t size );
void  free( void* p );

alloc_handler	 alloc_hook( alloc_handler function );
realloc_handler  realloc_hook( realloc_handler function );
free_handler	 free_hook( free_handler function );


}
#endif