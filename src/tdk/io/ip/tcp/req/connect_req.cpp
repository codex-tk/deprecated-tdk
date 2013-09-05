#include <sys/epoll.h>
#include <tdk/io/ip/tcp/req/connect_req.hpp>
#include <tdk/io/ip/tcp/channel_linux.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

connect_req::connect_req( void ){

}

connect_req::~connect_req( void ) {

}

void connect_req::init( tdk::io::ip::tcp::channel* chan 
            , const std::vector< tdk::io::ip::address >& addrs )
{
    _address = addrs;
    _address_it = _address.begin();
    _channel = chan;
}

bool connect_req::address_next( void ) {
    if ( _address_it == _address.end() ) {
        return false;
    }
    ++_address_it;
    if ( _address_it == _address.end() ) {
        return false;
    }
    return true;
}

tdk::io::ip::address& connect_req::address( void ) {
    return *_address_it;
}


tdk::io::ip::tcp::channel& connect_req::channel( void ) {
    return *_channel;
}

}}}}

