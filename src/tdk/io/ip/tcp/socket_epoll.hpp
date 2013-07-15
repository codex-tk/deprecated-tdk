#ifndef __tdk_io_ip_tcp_socket_epoll_h__
#define __tdk_io_ip_tcp_socket_epoll_h__

#include <tdk/io/ip/socket.hpp>
#include <tdk/io/engine_epoll.hpp>
#include <tdk/io/ip/tcp/operation/connect_operation_epoll.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class connect_operation;

class socket : public tdk::io::ip::socket {
public:
	socket( tdk::io::engine& engine );
	~socket( void );
	tdk::io::engine& engine( void );

    void handle_event( int event );
    
    template < typename T_handler >
    void async_connect( const tdk::io::ip::address& addr 
            , const T_handler& h );



private:
	tdk::io::engine* _engine;
    tdk::io::engine::context _context;

    int _state;
};

template < typename T_handler >
void socket::async_connect( const tdk::io::ip::address& addr 
            , const T_handler& h )
{
    typedef tdk::io::detail::operation_impl< T_handler 
                , tdk::io::ip::tcp::connect_operation 
                        , detail::dispatcher_error_code
                            > connect_operation_impl;
    std::vector< tdk::io::ip::address > addrs;
    addrs.push_back( addr );
/*    for( auto it : addrs ) {
        printf( "ADDR!! %s\r\n" , it.ip_address().c_str());
    }*/
    connect_operation_impl* impl( new connect_operation_impl( h , *this , addrs ));
    impl->process();
}



}}}}




#endif
