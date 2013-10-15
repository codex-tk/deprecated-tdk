#ifndef __tdk_io_ip_tcp_acceptor_linux_h__
#define __tdk_io_ip_tcp_acceptor_linux_h__

#include <tdk/loop/loop.hpp>
#include <tdk/io/ip/socket.hpp>
#include <tdk/io/ip/tcp/req/accept_req.hpp>

namespace tdk {
namespace io{
namespace ip{
namespace tcp {
/*
class acceptor {
public:
    typedef tdk::io::ip::socket socket_layer_type;
    acceptor( tdk::loop& loop );
    ~acceptor( void );

    tdk::loop& loop( void );
    socket_layer_type& socket_layer( void );

    bool open( const tdk::io::ip::address& addr 
            , tdk::req_handle::callback_type cb
            , void* data );
    void close( void );


    void handle_accept( int evt );
private:
    tdk::loop* _loop;
    tdk::epoll::event_handler _io_handler;
    tdk::io::ip::socket _socket;
    tdk::io::ip::tcp::accept_req _accept_req;
};*/

}}}}


#endif
