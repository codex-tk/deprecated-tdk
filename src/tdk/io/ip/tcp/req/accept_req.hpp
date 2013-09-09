#ifndef __tdk_io_ip_tcp_accept_req_h__
#define __tdk_io_ip_tcp_accept_req_h__

#include <tdk/loop/loop.hpp>
#include <tdk/io/ip/address.hpp>

namespace tdk {
namespace io{
namespace ip{
namespace tcp {

class accept_req : public tdk::io_req_handle {
public:
    accept_req( void );
    ~accept_req( void );

    int accepted_fd( void );
    void accepted_fd( int fd );

    tdk::io::ip::address& address();
private:
    int _fd;
    tdk::io::ip::address _address;
};

}}}}

#endif
