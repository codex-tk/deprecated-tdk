#include "stdafx.h"
#include <tdk/tdk.hpp>
#include <tdk/crypto/seed.hpp>
#include <tdk/crypto/seed/SEED_KISA.hpp>

namespace tdk {
namespace crypto {

seed::seed( void ){
    memset( roundKey_ , 0x00 , sizeof( DWORD ) * 32 );
}

seed::~seed( void ){
}

int seed::open( void* userKey , int size ){
    int keySize = std::min( size , 16 );
    unsigned char key[16];
    memset( key , 0x00 , sizeof( unsigned char ) * 16 );
    memcpy( key , userKey , keySize );

    SeedRoundKey( roundKey_ , key );
    return 0;
}

void seed::close( void ){
    memset( roundKey_ , 0x00 , sizeof( DWORD ) * 32 );
}

bool seed::encrypt( void* data , int size ){
    if ( size % 16 != 0 )  
        return false;
    
    int dataSize = size;
    unsigned char* dataPtr = static_cast< unsigned char* >( data );
    int writeSize = 0;
    while ( dataSize >= 16 ) {
        SeedEncrypt( dataPtr + writeSize , roundKey_ );
        dataSize -= 16;
        writeSize += 16;
    }   
    return true;
}

bool seed::decrypt( void* data , int size ){
    if ( size % 16 != 0 )  
        return false;

    int dataSize = size;
    unsigned char* dataPtr = static_cast< unsigned char* >( data );
    int writeSize = 0;
    while ( dataSize >= 16 ) {
        SeedDecrypt( dataPtr + writeSize , roundKey_ );
        dataSize -= 16;
        writeSize += 16;
    }   
    return true;
}


}
}
