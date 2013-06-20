#ifndef __tdk_CryptoSeed128_h__
#define __tdk_CryptoSeed128_h__

namespace tdk {
namespace crypto {

class seed{
public: 
    seed( void );
    ~seed( void );

    // userKey 는 128 bit 만 사용 한다.
    // 성공시 0 실패시 -1
    int open( void* userkey , int size );
    void close( void );
    
    // 128 bit 단위
    bool encrypt( void* data , int size );
    // 128 bit 단위
    bool decrypt( void* data , int size );

private:
    DWORD roundKey_[32];
};


}}

#endif