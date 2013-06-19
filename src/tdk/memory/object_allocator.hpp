#ifndef __tdk_memory_object_allocator_h__
#define __tdk_memory_object_allocator_h__

namespace tdk { namespace memory {

class object_allocator {
public:
	//
    static void* operator new ( std::size_t sz );
    static void  operator delete ( void* ptr );
    // replacement 
    static void* operator new ( std::size_t sz , void* p);
    static void  operator delete( void* , void* );
};


}}

#endif