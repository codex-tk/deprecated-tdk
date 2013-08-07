#ifndef __tdk_CryptoSeed128_h__
#define __tdk_CryptoSeed128_h__

#include <tdk/crypto/seed/SEED_KISA.hpp>

namespace tdk {
namespace crypto {

class seed{
public: 
    struct block{
        uint8_t data[16];
    };
    struct block_ref{
        uint8_t* data; // 16 byte
    };

    seed( void );
    ~seed( void );

    void open( const block& key );
    void close( void );
    
    void encrypt( block& b );
    void decrypt( block& b );

    void encrypt( block_ref& b );
    void decrypt( block_ref& b );

    int encrypt_size( int plain_sz );
    bool encrypt_cbc( uint8_t* plain 
            , int plain_sz
            , uint8_t* cipher
            , int* cipher_sz );

    bool decrypt_cbc( uint8_t* cipher
            , int cipher_sz
            , uint8_t* plain
            , int* plain_sz );
private:
    DWORD roundKey_[32];
};


}}

#endif
