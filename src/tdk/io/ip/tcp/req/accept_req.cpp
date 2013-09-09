#include <tdk/io/ip/tcp/req/accept_req.hpp>

namespace tdk {
namespace io{
namespace ip{
namespace tcp {

accept_req::accept_req( void ){

}

accept_req::~accept_req( void ) {
    
}

int accept_req::accepted_fd( void ) {
    return _fd;
}

void accept_req::accepted_fd( int fd ) {
    _fd = fd;
}

tdk::io::ip::address& accept_req::address( void ) {
    return _address;
}

}}}}

