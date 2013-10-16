#include "stdafx.h"
#include <tdk/crypto/padding.hpp>

namespace tdk{
namespace crypto{

void padding::pkcs5( tdk::buffer::memory_block& mb , uint8_t align ) {
    return pkcs5( mb , mb.length() , align );
}

void padding::pkcs5( tdk::buffer::memory_block& mb , std::size_t datasz , uint8_t align ) {
    std::size_t aligned_size = datasz + ( align - ( datasz % align ));
    uint8_t padding = static_cast<uint8_t>(aligned_size - datasz);
    for ( int i = 0 ; i < padding ; ++i ) {
        mb.write( &padding , 1 );
    }
}

std::size_t padding::pkcs5_data_size( uint8_t* data , std::size_t datasz ) {
    uint8_t padding = data[ datasz - 1 ];
    return datasz - padding;
}

}}

