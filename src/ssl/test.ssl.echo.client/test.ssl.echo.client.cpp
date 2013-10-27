// test.ssl.echo.server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#pragma comment ( lib , "ssleay32" )
#pragma comment ( lib , "libeay32" )
#pragma comment ( lib , "ws2_32" )

/* serv.cpp  -  Minimal ssleay server for Unix
   30.9.1996, Sampo Kellomaki <sampo@iki.fi> */
/* mangled to work with SSLeay-0.9.0b and OpenSSL 0.9.2b
   Simplified to be even more minimal
   12/98 - 4/99 Wade Scholine <wades@mail.cybg.com> */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include <sys/types.h>
#include <openssl/rsa.h>       /* SSLeay stuff */
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

/* define HOME to be dir for key and cert files... */
//#define HOME "./"

/* 서버 인증서 */
//#define CERTF  HOME "server-req.pem"
/* 서버 비밀키 */
//#define KEYF  HOME  "server-key.pem"

#define CHK_NULL(x) if ((x)==NULL) exit (1)
#define CHK_ERR(err,s) if ((err)==-1) { perror(s); exit(1); }
#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit(2); }

// SSL 핸드쉐이크 메시지교환 과정을 알려주는 콜벡함수
void  ssl_info_callback(const SSL *s, int where, int ret);

int pem_password(char *buf, int size, int rwflag, void *userdata) {
	//strncpy_s( buf , "1331" , size );
	strncpy(buf, "1331",size);
	buf[size - 1] = '\0';
	return(strlen(buf));
}
// 화면에 표시 하기 위한 파일 BIO생성
BIO * errBIO;

int main (int argc, char **argv)
{
	WSADATA ws;
	WSAStartup( MAKEWORD(2,2) , &ws );
    int err;
    int sd;
    struct sockaddr_in sa;

    const SSL_METHOD *meth;
    SSL_CTX* ctx;
    SSL*     ssl;
    X509*    client_cert;

    char*    str;
    char     buf [4096];

    /* SSL preliminaries. We keep the certificate and key with the context. */
    // 모든 에러 스트링 로드
    SSL_load_error_strings();
    // 모든 알고리즘 로드
    SSLeay_add_ssl_algorithms();
    // SSL 버전3 프로토콜 사용
    meth = SSLv3_method();
    //meth = TLSv1_server    // create a new SSL_CTX object as framework for TLS/SSL enabled functions
    // SSL 컨텍스트 생성
    ctx = SSL_CTX_new (meth);
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        exit(2);
    }
	
	sd = socket (AF_INET, SOCK_STREAM, 0);
    CHK_ERR(sd, "socket");

    memset (&sa, '\0', sizeof(sa));
    sa.sin_family      = AF_INET;
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");
    sa.sin_port        = htons (7543);          /* Server Port number */

    err = connect(sd, (struct sockaddr*) &sa, sizeof (sa));
	
	ssl = SSL_new( ctx );
	BIO* bio_recv = BIO_new( BIO_s_mem());
	BIO* bio_send = BIO_new( BIO_s_mem());
	SSL_set_bio( ssl , bio_recv , bio_send );
	SSL_set_connect_state( ssl );

	char* t_buf = new char[1024758];
	int r;
	r = SSL_read( ssl , nullptr , 0 );
	while ( true ) {
		
		printf( "read %d" , r );
		if( SSL_is_init_finished(ssl)){
			printf("Init finished\n");
			break;
		}
		while ( BIO_ctrl_pending( bio_send )){
			int bytes = BIO_read( bio_send , t_buf , 1 );
			if ( bytes > 0 ) {
				send( sd , t_buf , bytes , 0 );
			} else {
				break;
			}
		}
		r = recv( sd , t_buf , 1024768 , 0 );
		BIO_write( bio_recv , t_buf , r );
		r = SSL_read( ssl , t_buf , 1024768 );
	}

	client_cert = SSL_get_peer_certificate (ssl);


    if (client_cert != NULL)
    {
        printf ("Server certificate:\n");

        str = X509_NAME_oneline (X509_get_subject_name (client_cert), 0, 0);
        CHK_NULL(str);
        printf ("\t subject: %s\n", str);
        OPENSSL_free (str);

        str = X509_NAME_oneline (X509_get_issuer_name  (client_cert), 0, 0);
        CHK_NULL(str);
        printf ("\t issuer: %s\n", str);
        OPENSSL_free (str);

        /* We could do all sorts of certificate verification stuff here before
           deallocating the certificate. */

        X509_free (client_cert);
    }
    else
    {
        printf ("Client does not have certificate.\n");
    }

	int len = SSL_write( ssl , t_buf , 5 );
	while ( BIO_ctrl_pending( bio_send )){
		int bytes = BIO_read( bio_send , t_buf , 1024768 );
		if ( bytes > 0 ) {
			send( sd , t_buf , bytes , 0 );
		}
	}

	char* recv_buffer = new char[10247680];
	int recv_size = 0;

	while ( true ) {
		r = recv( sd , recv_buffer + recv_size , 4096 , 0 );
		printf( "Socket read %d\n" , r );
		if ( r > 0 ) {
			recv_size += r;
			int wr = BIO_write( bio_recv , recv_buffer , recv_size );
			if ( wr < 0 && !BIO_should_retry( bio_recv )) {
				printf("Error\n");
				return 0;
			} 
			if ( wr <= 0 ) {
				if ( !BIO_should_retry( bio_recv )) {
					printf("Error1\n");
					return 0;
				}else{
					printf( "Cont\n");
				}
			} else {
				if ( wr != recv_size ) {
					memmove( recv_buffer , recv_buffer + wr , recv_size - wr );
					recv_size -= wr;
					printf( "Remain %d\n" , recv_size );
				}else{
					printf( "No Reamin\n");
					recv_size = 0;
				}
				while ( true ) {
					r = SSL_read( ssl , t_buf , 1 );
					printf( "SSLread %d\n" , r );
					if ( r <= 0 )
						break;
				}
				
				
			}
		}
	}
	
	//BIO_write( bio_send , "I Want You" , strlen( "I Want You" ));




















	SSL_CTX_set_default_passwd_cb( ctx , &pem_password );
    // SSL 핸드쉐이크 메시지교환 과정을 알려주는 콜벡함수
    SSL_CTX_set_info_callback(ctx,ssl_info_callback);


    sd = socket (AF_INET, SOCK_STREAM, 0);
    CHK_ERR(sd, "socket");

    memset (&sa, '\0', sizeof(sa));
    sa.sin_family      = AF_INET;
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");
    sa.sin_port        = htons (1111);          /* Server Port number */

    err = connect(sd, (struct sockaddr*) &sa, sizeof (sa));
    CHK_ERR(err, "bind");

    ssl = SSL_new (ctx);
    CHK_NULL(ssl);

    // connect the SSL object with a file descriptor
    // 연결된 소켓과 SSL과의 연결
    SSL_set_fd (ssl, sd);
    // 가장 중요한 함수, 클라이언트와의 초기 협상과정, 즉 핸드쉐이크 과정을 수행
    printf ("SSL_connect start =========================\n");
	err = SSL_connect(ssl);
    CHK_SSL(err);
    printf ("SSL_connect end =========================\n");

    // Get the cipher - opt
    // 현재 클라이언트와 정의된 암호화 파라메터정보를 얻음
    printf ("SSL connection using %s\n", SSL_get_cipher (ssl));
    printf ("SSL connection using %s\n", SSL_CIPHER_get_name(SSL_get_current_cipher(ssl)));

    // Get client's certificate (note: beware of dynamic allocation) - opt
    client_cert = SSL_get_peer_certificate (ssl);


    if (client_cert != NULL)
    {
        printf ("Server certificate:\n");

        str = X509_NAME_oneline (X509_get_subject_name (client_cert), 0, 0);
        CHK_NULL(str);
        printf ("\t subject: %s\n", str);
        OPENSSL_free (str);

        str = X509_NAME_oneline (X509_get_issuer_name  (client_cert), 0, 0);
        CHK_NULL(str);
        printf ("\t issuer: %s\n", str);
        OPENSSL_free (str);

        /* We could do all sorts of certificate verification stuff here before
           deallocating the certificate. */

        X509_free (client_cert);
    }
    else
    {
        printf ("Client does not have certificate.\n");
    }

    // DATA EXCHANGE - Receive message and send reply.
    

	err = SSL_write (ssl, "I want you.", strlen("I want you."));
	CHK_SSL(err);

    err = SSL_read (ssl, buf, sizeof(buf) - 1);
    CHK_SSL(err);

    buf[err] = '\0';
    // 받은 데이터를 화면에 표시
    printf ("Got %d chars:'%s'\n", err, buf);

    /* Clean up. */
    closesocket (sd);
    SSL_free (ssl);
    SSL_CTX_free (ctx);
	getchar();
}

// SSL 핸드쉐이크 메시지교환 과정을 알려주는 콜벡함수
void  ssl_info_callback(const SSL *s, int where, int ret)
{
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
  return ;
}

/*

int _tmain(int argc, _TCHAR* argv[])
{
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();
	ERR_load_BIO_strings();

	BIO* server;
	BIO* client;
	char buf[256];

	server = BIO_new_accept( "9999" );
	if ( BIO_do_accept(server) <= 0 ) {
		ERR_print_errors_fp(stderr);
		return 0;
	}

	while(1)
	{
		// 듣기 소켓으로 부터 연결을 기다린다.
		printf("연결을 기다림\n");
		if (BIO_do_accept(server) <= 0)
		{
			ERR_print_errors_fp(stderr);
			exit(0);
		}
			// 만약 연결이루어 졌다면 
			// 해당 연결을 가지고 온다.
		client = BIO_pop(server);
		memset(buf, 0x00, 256);

			// 읽고 쓴다.
		if (BIO_read(client, buf, 255) <= 0)
		{
			perror("Read Error");
		}
		BIO_write(client, buf, 255);
		BIO_free(client);
	}
	return 0;
}

*/