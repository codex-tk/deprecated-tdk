#ifndef __tdk_io_engine_epoll_h__
#define __tdk_io_engine_epoll_h__

#include <tdk/tdk.hpp>

namespace tdk {
namespace io{

class engine {
public:
    engine( void );
    ~engine( void );
private:
    int _epoll_fd;
};


}}


#endif
