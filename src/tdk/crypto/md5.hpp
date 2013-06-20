#ifndef __tdk_crypto_MD5_h__
#define __tdk_crypto_MD5_h__

namespace tdk {
namespace crypto {
namespace detail {
/* MD5 context. */
typedef struct {
    unsigned int state[4];        /* state (ABCD) */
    unsigned int count[2];        /* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64];     /* input buffer */
} MD5_CTX;
}

class md5 {
public:
    md5( void );
    ~md5( void );
    
    void update( void* ptr , int size );
    void value( unsigned char* outValue /*[16]*/ );
    void reset( void );
private:
    detail::MD5_CTX context_;
};

}}

#endif