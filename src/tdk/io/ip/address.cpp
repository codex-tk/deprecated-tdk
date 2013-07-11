#include "stdafx.h"
#include <tdk/io/ip/address.hpp>
#include <tdk/tdk.hpp>
#if defined( _WIN32 )
#else
#include <netdb.h>
#endif

namespace tdk {
namespace io {
namespace ip {

namespace impl {
	/*
const TCHAR* inet_ntop(int af, const void *src, TCHAR *dst, socklen_t cnt) {
    sockaddr_storage storage;
    int size = 0;
	switch( af ) {
	case AF_INET:{
			sockaddr_in* addr = reinterpret_cast< sockaddr_in* >(&storage);
			addr->sin_family = AF_INET;
			size = sizeof( sockaddr_in );
			memcpy(&addr->sin_addr, src, sizeof(in_addr));
		}
		break;
	case AF_INET6:{
			sockaddr_in6* addr = reinterpret_cast< sockaddr_in6* >(&storage);
			addr->sin6_family = AF_INET6;
			size = sizeof( sockaddr_in6 );
			memcpy(&addr->sin6_addr, src, sizeof(in6_addr));
		}
		break;
	default:
		return dst;
	}
	if ( GetNameInfo(   reinterpret_cast< struct sockaddr*>(&storage) , 
                        size , 
                        dst , 
                        cnt , 
                        nullptr , 
                        0 , 
                        NI_NUMERICHOST ) != 0 ) {
        TDK_ASSERT( "[tdk::io::network::impl::inet_ntop] getnameinfo fail" );
    }
    return dst;
}*/

const char* inet_ntop(int af, const void *src, char *dst, socklen_t cnt) {
    sockaddr_storage storage;
    int size = 0;
	switch( af ) {
	case AF_INET:{
			sockaddr_in* addr = reinterpret_cast< sockaddr_in* >(&storage);
			addr->sin_family = AF_INET;
			size = sizeof( sockaddr_in );
			memcpy(&addr->sin_addr, src, sizeof(in_addr));
		}
		break;
	case AF_INET6:{
			sockaddr_in6* addr = reinterpret_cast< sockaddr_in6* >(&storage);
			addr->sin6_family = AF_INET6;
			size = sizeof( sockaddr_in6 );
			memcpy(&addr->sin6_addr, src, sizeof(in6_addr));
		}
		break;
	default:
		return dst;
	}
#if defined( _WIN32 )
    if ( GetNameInfoA(  
#else
    if ( getnameinfo(
#endif                
                        reinterpret_cast< struct sockaddr*>(&storage) , 
                        size , 
                        dst , 
                        cnt , 
                        nullptr , 
                        0 , 
                        NI_NUMERICHOST ) != 0 ) {
        //TDK_ASSERT( "[tdk::io::network::impl::inet_ntop] getnameinfo fail" );
    }
    return dst;
}


bool inet_pton( int af
	, const char* addr
	, int port
	, address& outaddr)
{
	struct addrinfo hints = {0,};
	struct addrinfo* res = nullptr;
	hints.ai_family = af;
	char port_string[12];
#if defined( _WIN32 )
	sprintf_s( port_string , "%d" , port );
#else
	sprintf( port_string , "%d" , port );
#endif
	int err = getaddrinfo( addr , port_string , &hints, &res);
    if ( err != 0) {
		return false;
    }
    if (res) {
		outaddr.set( res->ai_addr , static_cast<int>(res->ai_addrlen));
		freeaddrinfo(res);
		return true;
	}
	return false;;
}

}
		
address::address( void  ) : _length(sizeof(sockaddr_storage)) {
	memset( sockaddr() , 0x00 , sizeof( sockaddr_storage ));
#if defined (_DEBUG)
	_set_debug_info();
#endif
}

address::address( const address& rhs ) 
	: _length( rhs._length ) {
	memcpy( sockaddr() , rhs.sockaddr() , sizeof( sockaddr_storage ));
#if defined (_DEBUG)
	_set_debug_info();
#endif

}

address::address( const struct sockaddr* addr , 
				const int address_size ) 
	: _length( address_size ) 
{
	set( addr , address_size );
}

address::address( const unsigned long ipv4_addr 
	, const int port )
	: _length( sizeof( sockaddr_in ) )
{
	struct sockaddr_in addr;
	memset( &addr , 0x00 , sizeof( addr ));
    addr.sin_addr.s_addr = ipv4_addr;
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	memcpy( sockaddr() , &addr , sizeof( addr ));
#if defined (_DEBUG)
	_set_debug_info();
#endif
}

address::address( const in6_addr& ipv6_addr 
	, const int port ) 
	: _length( sizeof( sockaddr_in6 ) )
{
	struct sockaddr_in6 addr;
	memset( &addr , 0x00 , sizeof( addr ));
    addr.sin6_addr = ipv6_addr;
	addr.sin6_port = htons(port);
	addr.sin6_family = AF_INET6;
	memcpy( sockaddr() , &addr , sizeof( addr ));
#if defined (_DEBUG)
	_set_debug_info();
#endif
}

address::address( const std::string& addr 
	, const int port 
	, const int af )
	: _length( sizeof(sockaddr_storage) )
{
	if ( !impl::inet_pton( af , addr.c_str() , port , *this ) ) {
		set( address::any( port , af ).sockaddr() , address::any( port , af ).sockaddr_length());
	} 
#if defined (_DEBUG)
	_set_debug_info();
#endif
}

address& address::operator=( const address& rhs ) {
	set( rhs.sockaddr() , rhs.sockaddr_length() );
	return *this;
}
    
address::~address( void ){

}


void address::set( const struct sockaddr* addr , const int size ){
	_length = size;
	memcpy( sockaddr() , addr , _length);
#if defined (_DEBUG)
	_set_debug_info();
#endif
}

std::string address::ip_address( void ) const {
	char buffer[4096] = {0,};
	std::string ip = impl::inet_ntop( family()
		, &(((struct sockaddr_in*)sockaddr())->sin_addr) 
		, buffer
		, 4096);
	return ip;
}

int address::port( void ) const {
	return ntohs(((struct sockaddr_in*)sockaddr())->sin_port );
}

int	address::family( void ) const {
	return ((struct sockaddr_in*)sockaddr())->sin_family;
}

struct sockaddr* address::sockaddr( void ) const {
	return reinterpret_cast< struct sockaddr* >(  
		const_cast< sockaddr_storage* >( &_address ));
}
const int address::sockaddr_length( void ) const {
	return _length;
}
socklen_t*		address::sockaddr_length_ptr( void ) {
	return &_length;
}

bool address::resolve( 
	const std::string& ip_address , 
	int port , 
	std::vector< address >& addrs ) 
{
	struct addrinfo hint = {0,};
	struct addrinfo* res = nullptr;
	struct addrinfo* temp = nullptr;
	hint.ai_family	    = AF_UNSPEC;
	hint.ai_socktype	= SOCK_STREAM;
	hint.ai_flags		= AI_PASSIVE;
	char port_string[12];
#if defined( _WIN32 )
	sprintf_s( port_string , "%d" , port );
#else
	sprintf( port_string , "%d" , port );
#endif
	int err = getaddrinfo( ip_address.c_str() , port_string , &hint, &res);
    if ( err != 0) {
		//tdk::diagnostics::set_last_error( err );
		return false;
    }
	temp = res;
    while (temp) {
		address a( temp->ai_addr, static_cast<int>(temp->ai_addrlen));	
		addrs.push_back( a );
		temp = temp->ai_next;
	}
	if ( res ) {
		freeaddrinfo(res);
	}
	return true;	
}

#if defined ( _WIN32_WINNT ) && ( _WIN32_WINNT >= 0x0600 )
bool address::resolve( 
	const std::wstring& ip_address ,
	const std::wstring& port ,
	std::vector< address >& addresses ) 
{
	ADDRINFOW* res = nullptr;
	ADDRINFOW* ptr = nullptr;
	ADDRINFOW hint = {0,};
		
	hint.ai_family	    = AF_UNSPEC;
	hint.ai_socktype	= SOCK_STREAM;
	hint.ai_flags		= AI_PASSIVE;
	
	int rc = GetAddrInfoW( ip_address.c_str() , port.c_str() , &hint , &res );
	if ( rc != 0 )  {
		//tdk::diagnostics::set_last_error( rc );
		return false;
	}
	ptr = res;
	while ( ptr ) {
		address addr( ptr->ai_addr , static_cast<int>(ptr->ai_addrlen));
		addresses.push_back( addr );
		ptr = ptr->ai_next;
	}
	FreeAddrInfoW( res );
	return true;
}
#endif

address address::any( int port , int family ) {
	switch( family ) {
	case AF_INET6:
		return address( in6addr_any , port );
	default:
		return address( ( const unsigned long ) INADDR_ANY , port );
	}
}

address address::from( const std::string& addr , int port , int family ) {
	address resolved;
	if (!impl::inet_pton( 
		family 
		, addr.c_str()
		, port 
		, resolved ))
	{
		resolved = address::any( port , family );
	}
	return resolved;
}


bool operator==( const address& lhs , const address& rhs ) {
	if ( lhs.sockaddr_length() == rhs.sockaddr_length() ) {
		if ( memcmp( 
				lhs.sockaddr() ,
				rhs.sockaddr() , 
				rhs.sockaddr_length() ) == 0 )
			return true;
	}
	return false;
}

bool operator!=( const address& lhs , const address& rhs ) {
	return !( lhs == rhs );
}

}}}
