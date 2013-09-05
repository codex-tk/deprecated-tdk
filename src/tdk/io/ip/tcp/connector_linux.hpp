#ifndef __tdk_io_ip_tcp_connector_linux_h__
#define __tdk_io_ip_tcp_connector_linux_h__

#include <tdk/loop/loop.hpp>

namespace tdk {
namespace io{
namespace ip{
namespace tcp {

class connector {
public:
    connector( tdk::loop& loop );
    ~connector( void );

    tdk::loop& loop( void );

private:
    tdk::loop* _loop;
};

}}}}


#endif
