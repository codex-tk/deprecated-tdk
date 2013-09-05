#ifndef __tdk_read_req_h___
#define __tdk_read_req_h__

#include <vector>
#include <tdk/loop/loop.hpp>
#include <tdk/io/buffer_adapter.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class channel;    
class read_req : public tdk::io_req_handle {
public:
    read_req( void );
    ~read_req( void );

    void init( tdk::io::ip::tcp::channel* chan 
            , const tdk::io::buffer_adapter& buffer );

    tdk::io::buffer_adapter& buffer_adapter( void );

    tdk::req_handle* cross_thread_req( void );
    tdk::io::ip::tcp::channel& channel( void );
private:      
    tdk::req_handle _cross_thread_req;
    tdk::io::buffer_adapter _buffer;
    tdk::io::ip::tcp::channel* _channel;
};
  
}}}}

#endif
