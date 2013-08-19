#ifndef __tdk_util_zlib_h__
#define __tdk_util_zlib_h__

#include <tdk/buffer/memory_block.hpp>

namespace tdk {
namespace util {    

class zlib{
public:
    static bool compress( const tdk::buffer::memory_block& in , tdk::buffer::memory_block& out );
    static bool uncompress( const tdk::buffer::memory_block& in , tdk::buffer::memory_block& out );
private:    
};

}}

#endif
