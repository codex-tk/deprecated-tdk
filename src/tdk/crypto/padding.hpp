#ifndef __tdk_crypto_padding_h__
#define __tdk_crypto_padding_h__

#include <tdk/buffer/memory_block.hpp>

namespace tdk{
namespace crypto{

class padding{
public:    
    static void pkcs5( tdk::buffer::memory_block& mb , uint8_t align );
    static void pkcs5( tdk::buffer::memory_block& mb , uint32_t datasz , uint8_t align );
    static int  pkcs5_data_size( uint8_t* data , uint32_t datasz );
};


}}


#endif
