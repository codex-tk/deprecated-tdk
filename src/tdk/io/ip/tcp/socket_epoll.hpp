#ifndef __tdk_io_ip_tcp_socket_epoll_h__
#define __tdk_io_ip_tcp_socket_epoll_h__

#include <tdk/threading/spin_lock.hpp>
#include <tdk/util/list_node.hpp>
#include <tdk/io/ip/socket.hpp>
#include <tdk/io/engine_epoll.hpp>
#include <tdk/io/buffer_adapter.hpp>
#include <tdk/io/ip/tcp/operation/connect_operation_epoll.hpp>
#include <tdk/io/ip/tcp/operation/send_operation_epoll.hpp>
#include <tdk/io/ip/tcp/operation/recv_operation_epoll.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class socket : public tdk::io::ip::socket {
public:
    typedef tdk::slist_queue< tdk::io::operation > op_queue;

	socket( tdk::io::engine& engine );
	~socket( void );
	tdk::io::engine& engine( void );

    void handle_event( int event );
    
    template < typename T_handler >
    void async_connect( const tdk::io::ip::address& addr 
            , const T_handler& h );

    template < typename T_handler >
    void async_connect( const std::vector< tdk::io::ip::address >& addrs
            , const T_handler& h );

    template < typename T_handler >
    void async_send( const tdk::io::buffer_adapter& buf 
            , const T_handler& h );
    
    template < typename T_handler >
    void async_recv( const tdk::io::buffer_adapter& buf 
            , const T_handler& h );

    void async_connect( tdk::io::ip::tcp::connect_operation* op );
    void async_send( tdk::io::ip::tcp::send_operation* op );
    void async_recv( tdk::io::ip::tcp::recv_operation* op );

    void handle_connect( tdk::io::ip::tcp::connect_operation* op , int evt );
    void handle_error( op_queue& drain , const std::error_code& ec );
    void handle_send( op_queue& drain );
    void handle_recv( op_queue& drain );
    void handle_epollhup( op_queue& drain );
    void handle_epollerr( op_queue& drain );
    bool is_error_state( void );
private:
    tdk::io::engine* _engine;
    tdk::io::engine::context _context;
    
    tdk::threading::spin_lock _lock;
    op_queue _send_op_queue;
    op_queue _recv_op_queue;
    int _state;
};

template < typename T_handler >
void socket::async_connect( const tdk::io::ip::address& addr 
            , const T_handler& h )
{
    std::vector< tdk::io::ip::address > addrs;
    addrs.push_back( addr );
    this->async_connect( addrs , h );
}

template < typename T_handler >
void socket::async_connect( const std::vector< tdk::io::ip::address >& addrs
            , const T_handler& h ) 
{
    typedef tdk::io::detail::operation_impl< T_handler 
                , tdk::io::ip::tcp::connect_operation 
                , detail::dispatcher_error_code
    > connect_operation_impl;
    connect_operation_impl* impl( new connect_operation_impl( h , *this , addrs ));
    async_connect( impl );
}
template < typename T_handler >
void socket::async_send( const tdk::io::buffer_adapter& buf 
            , const T_handler& h )
{
    typedef tdk::io::detail::operation_impl< T_handler 
                 , tdk::io::ip::tcp::send_operation 
                 , detail::dispatcher_error_code_and_io_bytes
    > send_operation_impl;
    send_operation_impl* impl( new send_operation_impl( h , *this , buf  ));
    async_send( impl );
}

template < typename T_handler >
void socket::async_recv( const tdk::io::buffer_adapter& buf 
            , const T_handler& h )
{
    typedef tdk::io::detail::operation_impl< T_handler 
                 , tdk::io::ip::tcp::recv_operation 
                 , detail::dispatcher_error_code_and_io_bytes
    > recv_operation_impl;
    recv_operation_impl* impl( new recv_operation_impl( h , *this , buf  ));
    async_recv( impl );

}



}}}}




#endif
