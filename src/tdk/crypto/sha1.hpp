#ifndef __tdk_crypto_sha1_h__
#define __tdk_crypto_sha1_h__

namespace tdk {
namespace crypto {

class sha1 {
public:
    sha1( void );
    ~sha1( void );
    
    void update( void* ptr , int size );
    void value( unsigned char* outValue /*[20]*/ );
    void reset( void );

    struct CONTEXT{
        unsigned long state[5];
        unsigned long count[2];
        unsigned char buffer[64];
    };
private:
	CONTEXT _context;
};

}}


#endif