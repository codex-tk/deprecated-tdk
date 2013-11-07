#ifndef __ssl_filter_h__
#define __ssl_filter_h__


#if defined ( _TDK_OPENSSL_ENABLED_)

#include <tdk/io/ip/tcp/pipeline/filter.hpp>
#include <tdk/buffer/memory_block.hpp>
#include <openssl/ssl.h>

namespace tdk {
namespace ssl {

class filter : public tdk::io::ip::tcp::filter {
public:
	filter( SSL_CTX* ctx );
	virtual ~filter( void );

	virtual void on_accepted( const tdk::io::ip::address& addr );
	virtual void on_connected( void );
	virtual void on_read( tdk::buffer::memory_block& msg );
	virtual void do_write( tdk::buffer::memory_block& msg );

	bool is_fatal_error( int ret );
	void send_pending( void );
	bool read_data_write_bio( tdk::buffer::memory_block& msg );
	bool write_data_write_ssl( tdk::buffer::memory_block& msg );
	void begin_handshake( void );
	void end_handshake( void );

	X509* peer_certificate( void );
	void  free_peer_certificate( X509* x );

	SSL* ssl( void );
private:
	SSL* _ssl;
	BIO* _rbio;
	BIO* _wbio;
	int _handshake_state;
	tdk::io::ip::address _accept_addr;
};

}}

#endif

#endif
