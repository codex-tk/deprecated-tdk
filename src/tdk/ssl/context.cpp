#include "stdafx.h"
#include "context.hpp"

#include <openssl/err.h>
#include <openssl/x509v3.h>
#include <tdk/threading/spin_lock.hpp>
#include <vector>

namespace tdk {
namespace ssl {

void  ssl_info_callback(const SSL *s, int where, int ret);
int pem_password(char *buf, int size, int rwflag, void *userdata) ;

context::context(void)
	: _impl( SSL_CTX_new( SSLv23_method()))
{
	SSL_CTX_set_verify(_impl, SSL_VERIFY_NONE, nullptr);
	SSL_CTX_set_default_passwd_cb( _impl , &pem_password );
	SSL_CTX_set_default_passwd_cb_userdata( _impl , this );
	SSL_CTX_set_info_callback(_impl,ssl_info_callback);
}

context::~context(void)
{
	SSL_CTX_free( _impl );
}

bool context::use_certificate_file( const std::string& file ) {
	if ( SSL_CTX_use_certificate_file(_impl
		, file.c_str()
		, SSL_FILETYPE_PEM) <= 0)
	{
		return false;
    }
	return true;
}

bool context::use_private_key_file( const std::string& file 
								   , const std::string& pw ) {
	_private_key_password = pw;
	if ( SSL_CTX_use_PrivateKey_file(_impl
		, file.c_str()
		, SSL_FILETYPE_PEM) <= 0)
	{
		return false;
    }
	return true;
}

bool context::check_private_key( void ) {
	if (!SSL_CTX_check_private_key(_impl)) {
		return false;
	}
	return true;
}

SSL_CTX* context::impl( void ) {
	return _impl;
}

const std::string& context::password( void ) {
	return _private_key_password;
}

int pem_password(char *buf, int size, int rwflag, void *userdata) {
	context* ctx = static_cast< context* >( userdata );
	strncpy_s( buf , size , ctx->password().c_str() , ctx->password().length());
	buf[size - 1] = '\0';
	return(strlen(buf));
}

void  ssl_info_callback(const SSL *s, int where, int ret)
{
	/*
  char * writeString;
  int w;
  // 현재 어떤 메시지 교환 과정인지를 나타냄
  w = where & ~SSL_ST_MASK;

  // 클라이언트가 연결 했을 때
  if (w & SSL_ST_CONNECT)
    writeString="SSL_connect";
  // 서버가 연결을 받았을 때
  else if (w & SSL_ST_ACCEPT)
    writeString="SSL_accept";
  // 알 수 없는 경우
  else
    writeString="undefined";

    fprintf(stderr, "======== writeString = [%s]\n", writeString);

  // 일반적인 핸드쉐이크 프로토콜 메시지일 경우
  if (where & SSL_CB_LOOP)
  {
    // SSL_state_string_long(s) 함수로 부터 현재 진행되는 메시지가 무엇인지 표시
    BIO_printf(errBIO,"%s:%s\n",writeString,SSL_state_string_long(s));
    fprintf(stderr, "======== writeString = [%s], SSL_state_string_long(s) = [%s]\n", 
        writeString, SSL_state_string_long(s));
  }
  else if (where & SSL_CB_ALERT)
  { // alert 프로토콜일 경우
    writeString=(where & SSL_CB_READ)?"read":"write";
    BIO_printf(errBIO,"SSL3 alert %s:%s:%s\n",writeString,SSL_alert_type_string_long(ret),SSL_alert_desc_string_long(ret));
    fprintf(stderr, "======== writeString = [%s], SSL_alert_type_string_long(ret) = [%s], SSL_alert_desc_string_long(ret) = [%s]\n", 
          writeString, SSL_alert_type_string_long(ret), SSL_alert_desc_string_long(ret));
  }
  else if (where & SSL_CB_EXIT)
  { // 종료 과정일 경우
    if (ret == 0) {
      BIO_printf(errBIO,"%s:failed in %s\n",writeString,SSL_state_string_long(s));
      fprintf(stderr,"======== writeString = [%s], SSL_state_string_long(s) = [%s]\n", 
          writeString, SSL_state_string_long(s));
    }
    else if (ret < 0)
    {
      BIO_printf(errBIO,"%s:error in %s\n",writeString,SSL_state_string_long(s));
      fprintf(stderr,"======== writeString = [%s], SSL_state_string_long(s) = [%s]\n", 
          writeString, SSL_state_string_long(s));
    }
  }
  return ;*/
}


}}