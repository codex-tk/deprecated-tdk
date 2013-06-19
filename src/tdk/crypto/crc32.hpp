#ifndef __tdk_crypto_crc32_h__
#define __tdk_crypto_crc32_h__

namespace tdk {
namespace crypto {

class crc32 {
public:
	crc32(void);
	~crc32(void);

	void update( void* ptr , int size );
    unsigned int value( void );
    void reset( void );
private:
	unsigned int _crc32;
};

}}

#endif