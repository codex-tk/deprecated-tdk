#include "stdafx.h"

#if defined ( _TDK_OPENSSL_ENABLED_)

#include <tdk/ssl/filter.hpp>
#include <openssl/err.h>
#include <tdk/ssl/error_category.hpp>
#include <tdk/io/ip/tcp/channel.hpp>
#include <tdk/log/logger.hpp>

namespace tdk {
namespace ssl {
namespace {
const int k_handshake_connect = 0x01;
const int k_handshake_accept  = 0x02;
}

filter::filter( SSL_CTX* ctx ) 
	: _ssl( SSL_new( ctx ))
	, _rbio( BIO_new( BIO_s_mem()))
	, _wbio( BIO_new( BIO_s_mem()))
{
	SSL_set_bio( _ssl , _rbio , _wbio );
}

filter::~filter( void ){
	SSL_shutdown( _ssl );
	SSL_free( _ssl );
}

void filter::begin_handshake( void ) {
	int ret = SSL_read( _ssl , nullptr , 0 );
	if ( is_fatal_error( ret ) ){
		end_handshake();
		channel()->error_propagation( 
			ssl::ssl_error( SSL_get_error(_ssl, ret) ));
		return;
	}
	send_pending();
}

void filter::end_handshake( void ) {
	if ( _handshake_state == k_handshake_accept ) {
		tdk::io::ip::tcp::filter::on_accepted( _accept_addr );
	}
	if ( _handshake_state == k_handshake_connect ) {
		tdk::io::ip::tcp::filter::on_connected();
	}
	_handshake_state = 0;
}

void filter::on_accepted( const tdk::io::ip::address& addr ){
	_handshake_state = k_handshake_accept;
	SSL_set_accept_state( _ssl );
	begin_handshake();
}

void filter::on_connected( void ){
	_handshake_state = k_handshake_connect;
	SSL_set_connect_state( _ssl );
	begin_handshake();
}

void filter::send_pending( void ) {
	while ( BIO_ctrl_pending( _wbio )) {
		tdk::buffer::memory_block mb( 4096 );
		int ret = BIO_read( _wbio , mb.wr_ptr() , mb.space() );
		if ( ret <= 0 ) {
			if (is_fatal_error(ret)){				
				channel()->error_propagation(
					ssl::ssl_error( SSL_get_error(_ssl, ret)));
				end_handshake();
			}
			break;
		}else{
			mb.wr_ptr( ret );
			write_out_bound(mb);
		}
	}
}

bool filter::read_data_write_bio( tdk::buffer::memory_block& msg ) {
	while( msg.length() > 0 ) {
		int len = BIO_write( _rbio 
			, msg.rd_ptr()
			, msg.length());

		if ( len <= 0 ) {
			if ( !BIO_should_retry(_rbio)){
				channel()->error_propagation(
					ssl::ssl_error( SSL_get_error(_ssl, len)));
				end_handshake();
				return false;
			}
		} else {
			msg.rd_ptr( len );
		}
	}
	return true;
}

bool filter::write_data_write_ssl( tdk::buffer::memory_block& msg ) {
	while( msg.length() > 0 ) {
		int len = SSL_write( _ssl 
			, msg.rd_ptr()
			, msg.length());
		if ( len <= 0 ) {
			if ( is_fatal_error( len ) ) {
				channel()->error_propagation( 
					ssl::ssl_error( SSL_get_error(_ssl, len) ));
				return false;
			}
		} else {
			msg.rd_ptr( len );
		}
	}
	return true;
}

void filter::on_read( tdk::buffer::memory_block& msg ){
	if ( !read_data_write_bio( msg ) ) {
		return;
	}
	while ( true ) {
		tdk::buffer::memory_block mb(4096);
		int len = SSL_read( _ssl 
			, mb.wr_ptr() 
			, mb.space() );

		std::error_code ec;
		if ( len <= 0 ) {
			if ( is_fatal_error(len)){
				ec = ssl::ssl_error( SSL_get_error(_ssl, len));
			}
		}
		if (( _handshake_state != 0 ) && SSL_is_init_finished( _ssl ) ) {
			end_handshake();
		}
		if ( len <= 0 ) {
			if ( ec ) {
				end_handshake();
				channel()->error_propagation( ec );
				return;
			}
			break;
		} else {
			mb.wr_ptr( len );
			tdk::io::ip::tcp::filter::on_read( mb );
		}		
	}
	send_pending();
}

void filter::do_write( tdk::buffer::memory_block& msg ){
	write_data_write_ssl(msg);
	send_pending();
}

bool filter::is_fatal_error( int ret ) {
	switch(SSL_get_error(_ssl, ret)){
        case SSL_ERROR_NONE:
        case SSL_ERROR_WANT_READ:
        case SSL_ERROR_WANT_WRITE:
        case SSL_ERROR_WANT_CONNECT:
        case SSL_ERROR_WANT_ACCEPT:
            return false;
    }
	return true;
}


X509* filter::peer_certificate( void ) {
	return SSL_get_peer_certificate(_ssl);
}

void  filter::free_peer_certificate( X509* x ) {
	X509_free(x);
}

SSL* filter::ssl( void ) {
	return _ssl;
}

}}

#endif

