#ifndef __tdk_close_req_h___
#define __tdk_close_req_h__

#include <vector>
#include <tdk/loop/loop.hpp>
#include <tdk/io/ip/address.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class channel;    
class close_req : public tdk::io_req_handle{
public:
    close_req( void );
    ~close_req( void );
    
    void init( tdk::io::ip::tcp::channel* chan );
            
    tdk::io::ip::tcp::channel& channel( void );

    tdk::req_handle* channel_close_req( void );    
private:      
    tdk::req_handle _channel_close_req;
    tdk::io::ip::tcp::channel* _channel;
};
  
}}}}

#endif
