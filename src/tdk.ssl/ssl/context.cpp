#include "stdafx.h"
#include "context.hpp"

#include <openssl/err.h>
#include <openssl/x509v3.h>

namespace tdk {
namespace ssl {

context::context(void)
	: _impl( SSL_CTX_new( SSLv23_method()))
{
	SSL_CTX_set_verify(_impl, SSL_VERIFY_NONE, nullptr);
}


context::~context(void)
{
}

bool context::init( void ) {
	SSL_load_error_strings();
	SSL_library_init();
    ERR_load_BIO_strings();
    ERR_load_SSL_strings();
	return true;
}

}}
/* http://famellee.wordpress.com/2013/02/20/use-openssl-with-io-completion-port-and-certificate-signing/
bool session_process(session *psession)
{
    bool fatal_error_occurred = false;
    if(nullptr != psession->ssl)
    {
        if(psession->bytes_transferred[RECV] > 0)
        {
            int bytes = BIO_write(psession->bio[RECV], psession->socket_buffer[RECV], psession->bytes_transferred[RECV]);
            int ssl_error = ssl_get_error(psession->ssl, bytes);
            if(bytes == psession->bytes_transferred[RECV])
            {
                psession->bytes_transferred[RECV] = 0;
            }
            else if(!BIO_should_retry(psession->bio[RECV]))
            {
                fatal_error_occurred = true;
            }
        }
 
        if(psession->ssl_buffer_size[RECV] == 0)
        {
            int bytes = SSL_read(psession->ssl, psession->ssl_buffer[RECV], BUFFER_SIZE);
            int ssl_error = ssl_get_error(psession->ssl, bytes);
         
            if((HANDSHAKING == (psession->status & HANDSHAKING)) && SSL_is_init_finished(psession->ssl))
            {
                psession->status &= ~HANDSHAKING;
                psession->status |= CONNECTED;
 
                app_on_session_connect(psession);
            }
 
            if(bytes > 0)
            {
                psession->ssl_buffer_size[RECV] = bytes;
                app_on_session_recv(psession);
                psession->ssl_buffer_size[RECV] = 0;
            }
            else if(ssl_is_fatal_error(ssl_error))
            {
                fatal_error_occurred = true;
            }
        }
 
        if(psession->ssl_buffer_size[SEND] > 0)
        {
            int bytes = SSL_write(psession->ssl, psession->ssl_buffer[SEND], psession->ssl_buffer_size[SEND]);
            int ssl_error = ssl_get_error(psession->ssl, bytes);
            if(bytes == psession->ssl_buffer_size[SEND])
            {
                psession->ssl_buffer_size[SEND] = 0;
            }
            else if(ssl_is_fatal_error(ssl_error))
            {
                fatal_error_occurred = true;
            }
        }
 
        if(psession->wsabuf[SEND].len == 0 && (0 != psession->s_listening || BIO_pending(psession->bio[SEND])))
        {
            int bytes = BIO_read(psession->bio[SEND], psession->socket_buffer[SEND], BUFFER_SIZE);
            int ssl_error = ssl_get_error(psession->ssl, bytes);
            if(bytes > 0)
            {
                psession->wsabuf[SEND].len = bytes;
            }
            else if(ssl_is_fatal_error(ssl_error))
            {
                fatal_error_occurred = true;
            }
        }
 
        if(fatal_error_occurred)
            session_close(psession);
    }
 
    session_send(psession);
    session_recv(psession);
     
    return !fatal_error_occurred;
}
*/