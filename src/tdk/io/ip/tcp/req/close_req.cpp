#include <tdk/io/ip/tcp/req/close_req.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

close_req::close_req( void ){

}

close_req::~close_req( void ) {

}

void close_req::init( tdk::io::ip::tcp::channel* chan ){
    _channel = chan;
}

tdk::io::ip::tcp::channel& close_req::channel( void ) {
    return *_channel;
}

tdk::req_handle* close_req::channel_close_req( void ) {
    return &_channel_close_req;
}

}}}}

