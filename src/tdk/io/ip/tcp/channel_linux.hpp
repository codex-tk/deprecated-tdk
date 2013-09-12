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

    void open( int fd );
    
    template < typename T_handler >
    void async_read( const tdk::io::buffer_adapter& buffer 
            , const T_handler& handler  );

    template < typename T_handler >
    void async_write( const tdk::io::buffer_adapter& buffer 
            , const T_handler& handler  );
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

template < typename T_handler >
void channel::async_read( const tdk::io::buffer_adapter& buffer 
        , const T_handler& handler )
{
    class read_req_impl : public read_req {
    public:
        read_req_impl( const T_handler& handler )
            : _handler( handler )
        {
            set( _on_read , nullptr );          
        }

        ~read_req_impl( void ) {

        }

        void on_read( void ) {
            _handler( *this );
            delete this;
        }

        static void _on_read( tdk::req_handle* h ) {
            static_cast< read_req_impl* >(h)->on_read();
        }
    private:
        T_handler _handler;
    };
    read_req_impl* impl = new read_req_impl( handler );
    async_read( impl , buffer );
}


template < typename T_handler >
void channel::async_write( const tdk::io::buffer_adapter& buffer 
        , const T_handler& handler )
{
    class write_req_impl : public write_req {
    public:
        write_req_impl( const T_handler& handler )
            : _handler( handler )
        {
            set( _on_write , nullptr );          
        }

        ~write_req_impl( void ) {

        }

        void on_write( void ) {
            _handler( *this );
            delete this;
        }

        static void _on_write( tdk::req_handle* h ) {
            static_cast< write_req_impl* >(h)->on_write();
        }
    private:
        T_handler _handler;
    };
    write_req_impl* impl = new write_req_impl( handler );
    async_write( impl , buffer );
}


}}}}


#endif
