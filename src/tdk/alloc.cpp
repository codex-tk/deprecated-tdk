#include "stdafx.h"
#include <tdk/alloc.hpp>
#include <iostream>
namespace tdk {
namespace detail {

tdk::alloc_handler _alloc_handler = malloc;
tdk::realloc_handler _realloc_handler = realloc;
tdk::free_handler _free_handler = free;

}

void* alloc( std::size_t size ) {
	return detail::_alloc_handler(size);
}
void* realloc( void* p , std::size_t size ){
	return detail::_realloc_handler(p,size);
}
void  free( void* p ){
	return detail::_free_handler(p);
}

alloc_handler	 alloc_hook( alloc_handler function ) {
	std::swap( function , detail::_alloc_handler);
	return function;
}

realloc_handler  realloc_hook( realloc_handler function ) {
	std::swap( function , detail::_realloc_handler);
	return function;
}
free_handler	 free_hook( free_handler function ) {
	std::swap( function , detail::_free_handler);
	return function;
}


}
