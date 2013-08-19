#ifndef __tdk_CryptoSeed128_h__
#define __tdk_CryptoSeed128_h__

#include <tdk/crypto/seed/SEED_KISA.hpp>

namespace tdk {
namespace crypto {

class seed{
public: 
    struct block{
        uint8_t data[16];
        block( void ){ memset( data ,0x00,16);}
        block( const block& rhs ) { memcpy( data , rhs.data , 16 );}
        block& operator=( const block& rhs ){ 
            memcpy( data , rhs.data , 16 );
            return *this;
        }
    };
    struct block_ref{
        uint8_t* data; // 16 byte
    };

    seed( void );
    seed( const block& key );
    ~seed( void );

    void open( const block& key );
    void open( const char* key_msg );
    void close( void );
    
    void encrypt( block& b );
    void decrypt( block& b );

    void encrypt( block_ref& b );
    void decrypt( block_ref& b );
    void encrypt( uint8_t* in , std::size_t insz 
            , uint8_t* out );
    void decrypt( uint8_t* in , std::size_t insz 
            , uint8_t* out );

/*
    int encrypt_size( int plain_sz );
    bool encrypt_cbc( uint8_t* plain 
            , int plain_sz
            , uint8_t* cipher
            , int* cipher_sz );

    bool decrypt_cbc( uint8_t* cipher
            , int cipher_sz
            , uint8_t* plain
            , int* plain_sz );
            */
private:
    DWORD roundKey_[32];
};


}}

#endif
