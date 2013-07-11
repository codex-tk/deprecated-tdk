#include "stdafx.h"
#include <tdk/tdk.hpp>
#include <tdk/io/ip/socket.hpp>
#include <tdk/error/last_error.hpp>
#include <cassert>
#include <fcntl.h>


namespace tdk {
namespace io {
namespace ip {

socket::category< AF_INET , SOCK_STREAM , IPPROTO_TCP>  socket::tcp_v4;
socket::category< AF_INET6 , SOCK_STREAM , IPPROTO_TCP> socket::tcp_v6;
socket::category< AF_INET , SOCK_DGRAM , IPPROTO_UDP>   socket::udp_v4;
socket::category< AF_INET6 , SOCK_DGRAM , IPPROTO_UDP>  socket::udp_v6;

socket::socket( void )
	: _handle( INVALID_SOCKET ) {

}

socket::socket( SOCKET fd ) 
	: _handle( fd ) {

}

socket::socket( const socket& rhs )
	: _handle( rhs.handle() ) {

}

socket::~socket( void ){
}

socket& socket::operator=( const socket& rhs ) {
	_handle = rhs.handle();
	return *this;
}

bool socket::open( const int addressFamily , const int type , const int protocol ) {
#if defined(_WIN32) || defined(_WIN64)
	_handle = WSASocket( addressFamily , 
						 type , 
						 protocol , 
						 nullptr , 
						 0 , 
						 WSA_FLAG_OVERLAPPED );
#else
    _handle = ::socket( addressFamily , 
						 type , 
						 protocol );
#endif
	if ( _handle == INVALID_SOCKET ) {
		tdk::set_last_error( tdk::platform::error());
		TDK_ASSERT(!"[tdk::io::ip::socket::open()] socket create fail" );
		return false;
	}
	return true;
}

void socket::close( void ) {
	if ( _handle != INVALID_SOCKET ) {
#if defined(_WIN32) || defined(_WIN64)
	    closesocket( _handle );
#else
        ::close( _handle );   
#endif
    }
	_handle = INVALID_SOCKET;
}

bool socket::shutdown( const int type ) {
	if ( ::shutdown( handle() , type ) == SOCKET_ERROR ) {
		tdk::set_last_error( tdk::platform::error());
		return false;
	}
	return true;
}

bool socket::bind( const tdk::io::ip::address& addr ) {
	if ( ::bind( handle() 
			, addr.sockaddr() 
			, addr.sockaddr_length()) == SOCKET_ERROR )
	{
		tdk::set_last_error( tdk::platform::error());
		return false;
	}
	return true;
}

bool socket::listen( const int pending ) {
	if ( ::listen( handle() , pending ) == SOCKET_ERROR ){
		tdk::set_last_error( tdk::platform::error());
		TDK_ASSERT(!"[tdk::io::ip::socket::listen()] listen fail" );
		return false;
	}
	return true;
}

tdk::io::ip::address socket::local_address( void ) const  {
	tdk::io::ip::address address;
	int err = getsockname( handle() 
		, address.sockaddr()
		, address.sockaddr_length_ptr() );
	if ( err != 0 ) {
		tdk::set_last_error( tdk::platform::error());		
	}
	return address;
}

tdk::io::ip::address socket::remote_address( void ) const {
	tdk::io::ip::address address;
	int err = getpeername( handle() 
		, address.sockaddr()
		, address.sockaddr_length_ptr() );
	if ( err != 0 ) {
		tdk::set_last_error( tdk::platform::error());
	}
    return address;
}

bool socket::connect( const tdk::io::ip::address& address ) const {
	int err = ::connect( handle() , address.sockaddr() , address.sockaddr_length());
	if ( err != 0 ) {
		tdk::set_last_error( tdk::platform::error());
		return false;
	}
	return true;
}

socket socket::accept( tdk::io::ip::address& address ) const {
	socket fd(::accept( handle() , address.sockaddr() , address.sockaddr_length_ptr()));
	return fd;
}

int socket::recv( void* buf , int size  , const int flag ) const {
	return ::recv( handle() , static_cast<char*>(buf) , size , flag );
}

int socket::send( void* buf , int size , const int flag ) const {
	return ::send( handle() , static_cast<char*>(buf) , size ,flag );
}

int socket::recvfrom( void* buf 
	, int size 
	, tdk::io::ip::address& address 
	, const int flag ) const 
{
	return ::recvfrom( handle() ,  static_cast<char*>(buf) , size  , 
		flag , address.sockaddr() , address.sockaddr_length_ptr() );
}

int socket::sendto( void* buf 
	, int size 
	, const tdk::io::ip::address& address 
	, const int flag ) const 
{
	return ::sendto( handle() ,  static_cast<char*>(buf) , size  , 
		flag , address.sockaddr() , address.sockaddr_length() );
}

bool socket::wait_for_recv( const tdk::time_span& wait ) const {
	if ( handle() == INVALID_SOCKET ) {
		tdk::set_last_error( tdk::platform::error());
		return false;
	}
	fd_set fdset;
	FD_ZERO(&fdset);
	FD_SET( handle() , &fdset);

    timeval tval = wait.to_timeval();
#if defined(_WIN32) || defined(_WIN64)
	int result = select( 0 , &fdset , NULL , NULL , &tval );
	if( result == SOCKET_ERROR ) {
		tdk::set_last_error( tdk::platform::error());
		return false;
	}
	if ( result == 0 ) {
		tdk::set_last_error( tdk::platform::timeout );
		return false;
	}
#else
    while( true ) {
        int ret = select( handle() + 1 ,&fdset , NULL ,  NULL , &tval );
        if ( ret > 0 )  break;
        if ( ret == -1 && errno == EINTR ) continue;
        if ( ret == 0 ) {
			//tdk::diagnostics::set_last_error( tdk::errors::timeout );
            return false;
        }
        if ( ret == -1 ) {
			//tdk::diagnostics::set_last_error( tdk::diagnostics::system_error());
            return false;
        }
    }
#endif
	if( !FD_ISSET( handle() , &fdset )) {
		tdk::set_last_error( tdk::platform::error());
		return false;
	}
	return true;
}

bool socket::wait_for_send( const tdk::time_span& wait ) const {
	SOCKET fd = handle();
	if ( fd == INVALID_SOCKET ) {
		tdk::set_last_error( tdk::platform::error());
		return false;
	}

	fd_set fdset;
	FD_ZERO(&fdset);
	FD_SET( handle() , &fdset);

    timeval tval = wait.to_timeval();
#if defined(_WIN32) || defined(_WIN64)
	int result = select( 0 , NULL , &fdset , NULL , &tval );
	if( result == SOCKET_ERROR ) {
		tdk::set_last_error( tdk::platform::error());
		return false;
	}
	if ( result == 0 ) {
		tdk::set_last_error( tdk::platform::timeout );
		return false;
	}  
#else
    while( true ) {
        int ret = select( handle() + 1 , NULL , &fdset ,  NULL , &tval );
        if ( ret > 0 )  break;
        if ( ret == -1 && errno == EINTR ) continue;
        if ( ret == 0 ) {
            //tdk::diagnostics::set_last_error( tdk::errors::timeout );
            return false;
        }
        if ( ret == -1 ) {
            //tdk::diagnostics::set_last_error( tdk::diagnostics::system_error());
            return false;
        }
    }    
#endif
	if( !FD_ISSET( fd , &fdset )) {
		tdk::set_last_error( tdk::platform::error());
		return false;
	}
	return true;
}

bool socket::connect_time_out( const tdk::io::ip::address& address ,  const tdk::time_span& wait ) {
	if ( connect( address )) {
		return true;
	}
#if defined ( _WIN32 )
    if ( WSAGetLastError() != WSAEWOULDBLOCK ) {
		return false;
	}
#else
    if ( errno != EINPROGRESS ){
        return false;
    }
#endif
	/*
	if ( tdk::diagnostics::get_last_error() != tdk::errors::in_progress ) {
		return false;
	}*/
    return wait_for_send( wait );
}

bool socket::send_time_out( void* buf , int size  , const tdk::time_span& wait ) {
    int sendsize = 0;
    unsigned char* buffer = static_cast< unsigned char* >(buf);
    int reqsize = size;
    while ( sendsize < reqsize ) {
        int ret = this->send(  buffer + sendsize , reqsize - sendsize );
        if ( ret <= 0 ) {
			tdk::set_last_error( tdk::platform::error());
			return false;
		}
        sendsize += ret;
        if ( !wait_for_send( wait ))
            return false;
    }
    return true;
}

int  socket::recv_time_out( void* buf , int size  , const tdk::time_span& wait ) {
    if ( wait_for_recv( wait ) ) {
        int ret = this->recv( buf , size );
		if ( ret <= 0 ) {
			tdk::set_last_error( tdk::platform::error());
		}
		return ret;
    }
    return SOCKET_ERROR;
}

bool socket::recv_fixed_size( void* buf , int size  , const tdk::time_span& wait ) {
    int recvsize = 0;
    unsigned char* buffer = static_cast< unsigned char* >(buf);
    int reqsize = size;
    while ( recvsize < reqsize ) {
        int ret = recv_time_out( buffer + recvsize , reqsize - recvsize  , wait );
        if ( ret <= 0 ) {
            return false;
        }
        recvsize += ret;
    }
    return true;
}

bool socket::connect_time_out( const tdk::io::ip::address& address , const int msec ) {
	return connect_time_out( address , tdk::time_span::from_milli_seconds( msec ));
}
bool socket::send_time_out( void* buf , int size , const int msec ) {
	return send_time_out( buf , size , tdk::time_span::from_milli_seconds( msec ));
}
int  socket::recv_time_out( void* buf , int size , const int msec ) {
	return recv_time_out( buf , size , tdk::time_span::from_milli_seconds( msec ));
}
bool socket::recv_fixed_size( void* buf , int size , const int msec ) {
	return recv_fixed_size( buf , size , tdk::time_span::from_milli_seconds( msec ));
}

SOCKET socket::handle( void ) const {
    return _handle;
}

SOCKET socket::handle( SOCKET fd ) {
	SOCKET old = _handle;
	_handle = fd;
	return old;
}

socket socket::invalid_socket(){
#if defined(_WIN32) || defined(_WIN64)
	return socket( INVALID_SOCKET );
#else
	return socket( -1 );
#endif
}

bool operator==( const socket& rhs , const socket& lhs ) {
    return rhs.handle() == lhs.handle();
}

bool operator!=( const socket& rhs , const socket& lhs ) {
    return rhs.handle() != lhs.handle();
}


socket::option::blocking::blocking( void ) : _option(0){

}

socket::option::blocking::~blocking( void ) {

}

bool socket::option::blocking::set_option( SOCKET fd ) {
#if defined(_WIN32) || defined(_WIN64)
	return ioctlsocket( fd , FIONBIO , &_option ) != SOCKET_ERROR;
#else
    int x = fcntl( fd , F_GETFL , 0);
    return fcntl(fd , F_SETFL , x & ~O_NONBLOCK ) != -1;
#endif
}


socket::option::non_blocking::non_blocking( void ) : _option(1){

}

socket::option::non_blocking::~non_blocking( void ) {

}

bool socket::option::non_blocking::set_option( SOCKET fd ) {
#if defined(_WIN32) || defined(_WIN64)
	return ioctlsocket( fd , FIONBIO , &_option ) != SOCKET_ERROR;
#else
    int x = fcntl( fd , F_GETFL , 0);
    return fcntl(fd , F_SETFL , x | O_NONBLOCK )!= -1;
#endif
}

namespace {
	::linger linger_value( int on_off , int liv ){
		::linger li;
		li.l_onoff = on_off;
		li.l_linger = liv;
		return li;
	}
}


socket::option::linger socket::option::linger_remove_time_wait( linger_value(1,0));

}}}
