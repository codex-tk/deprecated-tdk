#ifndef __tdk_ssl_h__
#define __tdk_ssl_h__

namespace tdk {
namespace ssl {

#if defined ( _TDK_OPENSSL_ENABLED_)

class open_ssl {
public:
	static bool init( void );
private:

};

#endif
}}

#endif

