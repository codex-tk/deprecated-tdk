#ifndef __tdk_crypto_base64_h__
#define __tdk_crypto_base64_h__

#include <string>

namespace tdk {
namespace crypto {

class base64{
public:
	base64(void);
	~base64(void);
	static std::string encode( const std::string& msg );
	static std::string encode( void* ptr , uint32_t size);
	static int decode( const std::string& src , void* ptr , uint32_t size );
};

}}

#endif