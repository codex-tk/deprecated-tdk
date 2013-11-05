#include "stdafx.h"
#include <tdk/tdk.hpp>
#include <tdk/crypto/seed.hpp>
#include <tdk/crypto/seed/SEED_KISA.hpp>
namespace tdk {
namespace crypto {
namespace detail {

static const int SEED_BLOCK_SIZE = 16;

}

seed::seed( void ){
    memset( roundKey_ , 0x00 , sizeof( DWORD ) * 32 );
}

seed::seed( const block& key ){
    open( key );
}

seed::seed( const char* key_msg  ) {
	block b;
    memset( b.data , 0x00 , 16 );
	strncpy((char*) b.data , key_msg , 16 );
    open( b );
}

seed::~seed( void ){
}

void seed::open( const char* key_msg ) {
    block b;
    memset( b.data , 0x00 , 16 );
	strncpy((char*) b.data , key_msg , 16 );
    open( b );
}

void seed::open( const seed::block& key_block  ){
    close();
	memset( roundKey_ , 0x00 , sizeof( DWORD ) * 32 );
    SeedRoundKey( roundKey_ ,const_cast<uint8_t*>( key_block.data ) );
}

void seed::close( void ){
    memset( roundKey_ , 0x00 , sizeof( DWORD ) * 32 );
}

void seed::encrypt( seed::block& b ){
    SeedEncrypt( b.data , roundKey_ );
}

void seed::decrypt( seed::block& b ){
    SeedDecrypt( b.data , roundKey_ );
}

void seed::encrypt( seed::block_ref& b ) {
    SeedEncrypt( b.data , roundKey_ );
}

void seed::decrypt( seed::block_ref& b ){
    SeedDecrypt( b.data , roundKey_ );
}

void seed::encrypt( uint8_t* in , std::size_t insz 
            , uint8_t* out ) 
{
    if ( in != out ) {
        memcpy( out , in , insz );
    }
    std::size_t offset = 0;
    while ( offset < insz ) {
        SeedEncrypt( out + offset , roundKey_);
        offset += detail::SEED_BLOCK_SIZE;
    }
}

void seed::decrypt( uint8_t* in , std::size_t insz 
            , uint8_t* out )
{
    if ( in != out ) {
        memcpy( out , in , insz );
    }
    std::size_t offset = 0;
    while ( offset < insz ) {
        SeedDecrypt( out + offset , roundKey_);
        offset += detail::SEED_BLOCK_SIZE;
    }

}
/*
int seed::encrypt_size( int plain_sz ) {
    return plain_sz + ( detail::SEED_BLOCK_SIZE - ( plain_sz % detail::SEED_BLOCK_SIZE ));
}

bool seed::encrypt_cbc( uint8_t* plain 
            , int plain_sz
            , uint8_t* cipher
            , int* cipher_sz )
{
    if ( cipher_sz == nullptr ) {
        return false;
    }
    int encrypted_size = encrypt_size(plain_sz);
    
    if ( *cipher_sz < encrypted_size ) 
        return false;
    
    uint8_t padding = encrypted_size - plain_sz;
    *cipher_sz = encrypted_size;
    
    if ( cipher != plain ) 
        memcpy( cipher , plain , plain_sz );

    for ( int i = plain_sz  
            ; i < encrypted_size
            ; ++i )
    {
        cipher[i]=padding;
    }

    printf( "enc padding %d\r\n" , padding );

    int encrypted = 0;
    while ( encrypted < encrypted_size ) {
        SeedEncrypt( cipher + encrypted , roundKey_);
        encrypted += detail::SEED_BLOCK_SIZE;
    }
    return true;
}

bool seed::decrypt_cbc( uint8_t* cipher
            , int cipher_sz
            , uint8_t* plain
            , int* plain_sz )
{
    int decrypted = 0;
    uint8_t dec_buffer[detail::SEED_BLOCK_SIZE];
    while (true){    
        memcpy( dec_buffer , cipher + decrypted , detail::SEED_BLOCK_SIZE );
        SeedDecrypt( dec_buffer , roundKey_ );
        if ( decrypted + detail::SEED_BLOCK_SIZE < cipher_sz ) {
            memcpy( plain + decrypted , dec_buffer , detail::SEED_BLOCK_SIZE );
        } else {
            uint8_t padding = dec_buffer[ detail::SEED_BLOCK_SIZE - 1 ];
            printf( "dec padding %d \r\n" , padding );
            int data_size = detail::SEED_BLOCK_SIZE - padding;
            memcpy( plain + decrypted , dec_buffer , data_size );
            *plain_sz = decrypted + data_size;
            break;
        }
        decrypted += detail::SEED_BLOCK_SIZE;
    }
    return true;        
}
*/
}
}
