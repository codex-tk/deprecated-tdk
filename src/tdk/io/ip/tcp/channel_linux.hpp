#ifndef __tdk_io_ip_tcp_channel_linux_h__
#define __tdk_io_ip_tcp_channel_linux_h__

#include <tdk/loop/loop.hpp>
#include <tdk/io/ip/socket.hpp>
#include <tdk/io/ip/tcp/req/connect_req.hpp>
#include <tdk/io/ip/tcp/req/close_req.hpp>
#include <tdk/io/ip/tcp/req/read_req.hpp>
#include <tdk/io/ip/tcp/req/write_req.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class channel {
public:
    typedef tdk::io::ip::socket socket_layer_type;
    channel( tdk::loop& loop );
    ~channel( void );

    tdk::loop& loop( void );
    
    void async_connect( connect_req* req 
            , const tdk::io::ip::address& addr );

    void async_connect( connect_req* req
            , const std::vector< tdk::io::ip::address >& addrs );

    void async_read( read_req* req 
            , const tdk::io::buffer_adapter& buffer );

    void async_write( write_req* req
            , const tdk::io::buffer_adapter& buffer );

    void async_close( close_req* req );

    socket_layer_type& socket_layer( void );
public:
    void do_connect( connect_req* req );
    void do_write( write_req* req );
    void do_read( read_req* req );
    void do_reg_req( void );

    void handle_connect( connect_req* req , int evt );
    void handle_close( close_req* req );
    void handle_io_event( int evt );
    bool handle_read( void );
    bool handle_write( void );
    void handle_complete( void );
    void handle_error( const std::error_code& err );
    
    void handle_reg( void );

    bool reg( void );
    bool is_closed( void );
private:
    tdk::io::ip::socket _socket;
    tdk::epoll::event_handler _io_handler;
    tdk::req_handle _reg_req;
    tdk::loop* _loop;
    tdk::slist_queue< tdk::req_handle > _read_reqs;
    tdk::slist_queue< tdk::req_handle > _write_reqs;
    tdk::slist_queue< tdk::req_handle > _complete_queue;
};

}}}}


#endif
