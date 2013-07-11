#ifndef __tdk_io_net_socket_h__
#define __tdk_io_net_socket_h__

#include <tdk/io/ip/address.hpp>
#include <tdk/time/time_span.hpp>
#if defined ( _WIN32 ) 

#elif defined( linux ) || defined ( __linux )
typedef int SOCKET;
static const int INVALID_SOCKET = -1;
static const int SOCKET_ERROR = -1;

#include <netinet/tcp.h>
#elif defined ( __MACOSX__ ) || defined ( __APPLE__ ) 

#else

#endif


namespace tdk {
namespace io {
namespace ip {



class socket {
public:
	template < int V_family , int V_type , int V_protocol >
	class category {
	public:
		category(){}
		~category(){}
		int family() const{
			return V_family;
		}
		int type() const{
			return V_type;
		}
		int protocol() const{
			return V_protocol;
		}
	};

	static category< AF_INET , SOCK_STREAM , IPPROTO_TCP> tcp_v4;
	static category< AF_INET6 , SOCK_STREAM , IPPROTO_TCP> tcp_v6;
	static category< AF_INET , SOCK_DGRAM , IPPROTO_UDP> udp_v4;
	static category< AF_INET6 , SOCK_DGRAM , IPPROTO_UDP> udp_v6;

public:
	socket( void );
	socket( SOCKET fd );
	socket( const socket& rhs );
	~socket( void );

	socket& operator=( const socket& rhs );

	bool open_tcp( int family ) {
		if ( family == AF_INET6) {
			return open( tcp_v6 );
		}
		return open( tcp_v4 );
	}

	template < typename T_category >
	bool open( const T_category& cate ) {
		return open( cate.family() , cate.type() , cate.protocol());
	}

	bool open( const int addressFamily , const int type , const int protocol );
	void close( void );

	bool shutdown( const int type );

	bool bind( const tdk::io::ip::address& addr );
	bool listen( const int pending = SOMAXCONN );

	tdk::io::ip::address local_address( void ) const;
	tdk::io::ip::address remote_address( void ) const;

	bool connect( const tdk::io::ip::address& address ) const;
	socket accept( tdk::io::ip::address& address ) const;

	int recv( void* buf , int size , const int flag = 0 ) const;
	int send( void* buf , int size , const int flag = 0 ) const;

	int recvfrom(void* buf , int size , tdk::io::ip::address& address , const int flag  = 0 ) const;
	int sendto(  void* buf , int size ,  const tdk::io::ip::address& address , const int flag  = 0 ) const;

	bool wait_for_recv( const tdk::time_span& wait ) const;
	bool wait_for_send( const tdk::time_span& wait ) const;

    bool connect_time_out( const tdk::io::ip::address& address ,  const tdk::time_span& wait );
    bool send_time_out( void* buf , int size , const tdk::time_span& wait );
    int  recv_time_out( void* buf , int size , const tdk::time_span& wait );
    bool recv_fixed_size( void* buf , int size , const tdk::time_span& wait );

	bool connect_time_out( const tdk::io::ip::address& address , const int msec );
    bool send_time_out( void* buf , int size , const int msec );
    int  recv_time_out( void* buf , int size , const int msec );
    bool recv_fixed_size( void* buf , int size , const int msec );
    
	SOCKET handle( void ) const;
	SOCKET handle( SOCKET fd );
	template < typename T_option >
	bool set_option( T_option& opt ) {
		if ( handle() != INVALID_SOCKET ) {
			return opt.set_option( handle() );
		}
		return false;
	}

	template < typename T_option >
	bool get_option( T_option& opt ) {
		if ( handle() != INVALID_SOCKET ) {
			return opt.get_option( handle() );
		}
		return false;
	}
public:
    static socket invalid_socket();
private:
	SOCKET _handle;
public:
    class option {
    private:
	    template < typename T , int level , int opt >
	    class value_option {
	    public:
		    value_option( void ) {
		    }
		    explicit value_option( const T& t ) : _option(t) {
		    }
		    ~value_option( void ) {
		    }
		    bool set_option( SOCKET fd ) {
			    return setsockopt( fd , 
					       level , 
					       opt , 
					       reinterpret_cast< char*>(&_option) , 
					       sizeof( _option ) ) != SOCKET_ERROR;
		    }
		    bool get_option( SOCKET fd ) {
			    socklen_t size = sizeof( _option );
			    return getsockopt( fd,
							       level , 
							       opt , 
							       reinterpret_cast< char*>(&_option) ,
							       &size ) != SOCKET_ERROR;
		    }

		    T& value( void ) {
			    return _option;
		    }

		    void value( const T& value ) {
			    _option = value;
		    }

	    private:
		    T _option;
	    };
    public:
	    class blocking {
	    public:
		    blocking( void );
		    ~blocking( void );
		    bool set_option( SOCKET fd );
	    private:
		    unsigned long _option;
	    };

	    class non_blocking {
	    public:
		    non_blocking( void );
		    ~non_blocking( void );
		    bool set_option( SOCKET fd );
	    private:
		    unsigned long _option;
	    };
	
        typedef value_option< int       , SOL_SOCKET	, SO_REUSEADDR			> reuse_address;
        typedef value_option< int       , SOL_SOCKET	, SO_BROADCAST			> broad_cast;
        typedef value_option< int		, SOL_SOCKET	, SO_RCVBUF				> recv_buffer;
        typedef value_option< int		, SOL_SOCKET	, SO_RCVTIMEO			> recv_time;
        typedef value_option< int		, SOL_SOCKET	, SO_SNDBUF				> send_buffer;
        typedef value_option< int		, SOL_SOCKET	, SO_SNDTIMEO			> send_time;
		typedef value_option< struct linger	, SOL_SOCKET	, SO_LINGER				> linger;
		
		static linger linger_remove_time_wait;

		typedef value_option< uint8_t	, IPPROTO_IP	, IP_MULTICAST_LOOP		> multi_cast_loop;
		typedef value_option< uint8_t	, IPPROTO_IP	, IP_MULTICAST_TTL		> multi_cast_ttl;
		typedef value_option< in_addr	, IPPROTO_IP	, IP_MULTICAST_IF		> multi_cast_if;
		typedef value_option< ip_mreq	, IPPROTO_IP	, IP_ADD_MEMBERSHIP		> multi_cast_join;
		typedef value_option< ip_mreq	, IPPROTO_IP	, IP_DROP_MEMBERSHIP	> multi_cast_drop;


		typedef value_option< uint8_t	, IPPROTO_IPV6	, IPV6_MULTICAST_LOOP	> multi_cast_loop_ipv6;
		typedef value_option< uint8_t	, IPPROTO_IPV6	, IPV6_MULTICAST_HOPS	> multi_cast_ttl_ipv6;
		typedef value_option< in6_addr	, IPPROTO_IPV6	, IPV6_MULTICAST_IF		> multi_cast_if_ipv6;
		typedef value_option< ipv6_mreq	, IPPROTO_IPV6	, IPV6_ADD_MEMBERSHIP	> multi_cast_join_ipv6;
		typedef value_option< ipv6_mreq	, IPPROTO_IPV6	, IPV6_DROP_MEMBERSHIP	> multi_cast_drop_ipv6;
		
		typedef value_option< int       , IPPROTO_TCP	, TCP_NODELAY		> tcp_no_delay;

#if defined(_WIN32) || defined(_WIN64)	
        typedef value_option< BOOL		, SOL_SOCKET	, SO_CONDITIONAL_ACCEPT		> conditional_accept;
        typedef value_option< int		, SOL_SOCKET	, SO_CONNECT_TIME			> connect_time;
        typedef value_option< SOCKET	, SOL_SOCKET	, SO_UPDATE_ACCEPT_CONTEXT	> update_accept_context;
#endif
    };
};

bool operator==( const socket& rhs , const socket& lhs );
bool operator!=( const socket& rhs , const socket& lhs ) ;

}}}


#endif
