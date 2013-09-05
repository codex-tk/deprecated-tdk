#include <sys/epoll.h>
#include <tdk/io/ip/tcp/req/write_req.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

write_req::write_req( void ){

}

write_req::~write_req( void ) {

}

void write_req::init( tdk::io::ip::tcp::channel* chan 
            , const tdk::io::buffer_adapter& buffer )
{
    _channel = chan;
    _buffer = buffer;
}

tdk::io::buffer_adapter& write_req::buffer_adapter( void ) {
    return _buffer;
}

tdk::io::buffer_adapter write_req::write_buffer( void ) {
    tdk::io::buffer_adapter buffer;
    const tdk::io::buffer_adapter::buffer_type* impl = _buffer.buffers();
    int count = _buffer.count();
    int writed = io_bytes();
    for ( int i = 0 ; i < count ; ++i ) {
        if ( impl[i].iov_len <= writed ) {
            writed -= impl->iov_len;
        } else {
            buffer.push_back( impl[i].iov_base + writed , impl[i].iov_len - writed );
            writed = 0;
        }
    }
    return buffer;
}

tdk::req_handle* write_req::cross_thread_req( void ){
    return &_cross_thread_req;
}

tdk::io::ip::tcp::channel& write_req::channel( void ) {
    return *_channel;
}

}}}}

