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
#include <tdk/io/ip/tcp/operation/accept_operation_epoll.hpp>
#include <bitset>
#include <limits>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class socket : public tdk::io::ip::socket {
public:
    static class _init {
    public:
        _init( void );
    } _initializer;
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

    template < typename T_handler >
    void async_accept( tdk::io::ip::tcp::socket& fd 
            , const T_handler& h );

    void async_connect( tdk::io::ip::tcp::connect_operation* op );
    void async_send( tdk::io::ip::tcp::send_operation* op );
    void async_recv( tdk::io::ip::tcp::recv_operation* op );
    void async_accept( tdk::io::ip::tcp::accept_operation* op );

    void handle_connect( tdk::io::ip::tcp::connect_operation* op , int evt );
    void handle_accept( int evt );
    void handle_error( const std::error_code& ec );
    void handle_send( void );
    void handle_recv( void );
    void handle_close( tdk::io::operation* op );
    
    void ctl_remain_events( void );

    bool open_accept( const  tdk::io::ip::address& addr );

private:
    bool _open( int fd );
    bool _write( tdk::io::ip::tcp::send_operation* op );
    bool _read( tdk::io::ip::tcp::recv_operation* op);
    int  _accept( tdk::io::ip::address& addr );
    void _drain( void );
    bool _ctl( int type , int evt ); 
    bool _ctl( int type , int evt , void* ctx );
private:
    tdk::io::engine* _engine;
    tdk::io::engine::context _context;
    
    tdk::threading::spin_lock _lock;
    op_queue _send_op_queue;
    op_queue _recv_op_queue;
    op_queue _accept_op_queue;
    op_queue _drain_op_queue;
    std::bitset< std::numeric_limits< uint32_t >::digits > _state;
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


template < typename T_handler >
void socket::async_accept( tdk::io::ip::tcp::socket& fd 
            , const T_handler& h )
{
    typedef tdk::io::detail::operation_impl< T_handler 
                 , tdk::io::ip::tcp::accept_operation 
                 , detail::dispatcher_error_code
    > accept_operation_impl;
    accept_operation_impl* impl( new accept_operation_impl( h , *this , fd ));
    async_accept( impl );
}

}}}}




#endif
