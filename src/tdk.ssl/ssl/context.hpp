#ifndef __tdk_ssl_h__
#define __tdk_ssl_h__

#include <openssl/ssl.h>

namespace tdk {
namespace ssl {

class context {
public:
	context(void);
	~context(void);
private:
	SSL_CTX* _impl;
public:
	static bool init( void );
};


}}

#endif

