#include <tdk/crypto/padding.hpp>

namespace tdk{
namespace crypto{

void padding::pkcs5( tdk::buffer::memory_block& mb , uint8_t align ) {
    return pkcs5( mb , mb.length() , align );
}

void padding::pkcs5( tdk::buffer::memory_block& mb , uint32_t datasz , uint8_t align ) {
    uint32_t aligned_size = datasz + ( align - ( datasz % align ));
    uint8_t padding = aligned_size - datasz;
    for ( int i = 0 ; i < padding ; ++i ) {
        mb.write( &padding , 1 );
    }
}

int padding::pkcs5_data_size( uint8_t* data , uint32_t datasz ) {
    uint8_t padding = data[ datasz - 1 ];
    return datasz - padding;
}

}}

