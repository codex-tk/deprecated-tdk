#include <tdk/io/ip/tcp/operation/send_operation_epoll.hpp>
#include <tdk/io/ip/tcp/socket_epoll.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

send_operation::send_operation( tdk::io::operation::callback cb 
		, tdk::io::ip::tcp::socket& fd
		, const tdk::io::buffer_adapter& buffer )
    : tdk::io::operation( cb )
    , _socket( &fd )
    , _buffer_adapter( buffer )
{
    
}

send_operation::~send_operation( void ) {

}

bool send_operation::end_operation( void ) {
    return true;
}

void send_operation::process( void ) {
}

tdk::io::ip::tcp::socket& send_operation::socket( void ) {
    return *_socket;
}

const tdk::io::buffer_adapter& send_operation::buffer_adapter( void ) {
    return _buffer_adapter;
}

tdk::io::buffer_adapter send_operation::write_buffer( void ) {
    tdk::io::buffer_adapter buffer;
    const tdk::io::buffer_adapter::buffer_type* impl = _buffer_adapter.buffers();
    int count = _buffer_adapter.count();
    std::size_t writed = io_bytes();
    for ( int i = 0 ; i < count ; ++i ) {
        if ( impl[i].iov_len <= writed ) {
            writed -= impl->iov_len;
        } else {
            buffer.push_back( static_cast< uint8_t* >(impl[i].iov_base) + writed
            		, impl[i].iov_len - writed );
            writed = 0;
        }
    }
    return buffer;
}

}}}}
