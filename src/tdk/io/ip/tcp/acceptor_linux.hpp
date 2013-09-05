#ifndef __tdk_io_ip_tcp_acceptor_linux_h__
#define __tdk_io_ip_tcp_acceptor_linux_h__

#include <tdk/loop/loop.hpp>

namespace tdk {
namespace io{
namespace ip{
namespace tcp {

class acceptor {
public:
    acceptor( tdk::loop& loop );
    ~acceptor( void );

    tdk::loop& loop( void );
private:
    tdk::loop* _loop;
};

}}}}


#endif
