#include <tdk/io/ip/tcp/acceptor_linux.hpp>
#include <sys/epoll.h>
#include <tdk/error/error_tdk.hpp>
#include <tdk/error/error_platform.hpp>

namespace tdk{
namespace io{
namespace ip{
namespace tcp{

void on_accept( tdk::req_handle* req ){
    acceptor* a = static_cast< acceptor* >( req->data() );
    a->handle_accept( static_cast< tdk::epoll::event_handler* >(req)->events());
}

acceptor::acceptor( tdk::loop& loop ) 
    : _loop( &loop ){
}
acceptor::~acceptor( void ) {
}

tdk::loop& acceptor::loop( void ) {
    return *_loop;
}

tdk::io::ip::tcp::acceptor::socket_layer_type& acceptor::socket_layer( void ) {
    return _socket;
}

bool acceptor::open( const tdk::io::ip::address& addr 
    , tdk::req_handle::callback_type cb
    , void* data ) 
{
    if ( !_socket.open_tcp( addr.family())) {
        return false;
    }
    tdk::io::ip::socket::option::reuse_address reuse;
    tdk::io::ip::socket::option::non_blocking nb;
    if ( !_socket.set_option( reuse )){
        return false;
    }
    if ( !_socket.set_option( nb ) ){
        return false;
    }
    if ( !_socket.bind( addr )){
        return false;
    }
    if ( !_socket.listen()){
        return false;
    }    
    _accept_req.set( cb , data );    
    _io_handler.set( on_accept , this );
    if ( _loop->implementation().reg( 
              _socket.handle()
            , EPOLLIN  
            , &_io_handler )){
        _loop->add_active();
        return true;
    }
    return false;
}

void acceptor::close( void ) {
    if ( _socket.handle() == INVALID_SOCKET ) {
        return;
    }
    _loop->remove_active();
    _socket.close();
}

void acceptor::handle_accept( int evt ) {
    if ( evt & EPOLLERR ){
        _accept_req.error( tdk::tdk_epoll_error);
        tdk::io::ip::socket::option::error sock_error;
        if ( socket_layer()
                .get_option( sock_error ) && sock_error.value()!= 0){
            _accept_req.error(tdk::platform::error( sock_error.value()));
        }
    } else if ( evt & EPOLLHUP ) {
        _accept_req.error( tdk::tdk_epoll_hang_up );
    } else if ( evt & EPOLLIN ) {
        int fd = 0;
        do {
            fd = accept( _socket.handle()
                    , _accept_req.address().sockaddr()
                    , _accept_req.address().sockaddr_length_ptr());
        } while (( fd < 0)  && ( errno == EINTR ));
        _accept_req.accepted_fd( fd );
        if ( fd < 0 ) {
            _accept_req.error( tdk::platform::error( errno ));
        } else {
            _accept_req.error( std::error_code());
        }
    } else {
        _accept_req.error( tdk::tdk_invalid_event );
    }
    _accept_req.invoke();
}

}}}}

