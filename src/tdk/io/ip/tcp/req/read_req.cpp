#include <sys/epoll.h>
#include <tdk/io/ip/tcp/req/read_req.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

read_req::read_req( void ){

}

read_req::~read_req( void ) {

}

void read_req::init( tdk::io::ip::tcp::channel* chan 
            , const tdk::io::buffer_adapter& buffer )
{
    _channel = chan;
    _buffer = buffer;
}

tdk::io::buffer_adapter& read_req::buffer_adapter( void ) {
    return _buffer;
}

tdk::req_handle* read_req::cross_thread_req( void ){
    return &_cross_thread_req;
}

tdk::io::ip::tcp::channel& read_req::channel( void ) {
    return *_channel;
}
}}}}

