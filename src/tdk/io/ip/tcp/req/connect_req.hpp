#ifndef __tdk_connect_req_h__
#define __tdk_connect_req_h__

#include <vector>
#include <tdk/loop/loop.hpp>
#include <tdk/io/ip/address.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class channel;    
class connect_req : public tdk::io_req_handle {
public:
    connect_req( void );
    ~connect_req( void );
    
    void init( tdk::io::ip::tcp::channel* chan 
            , const std::vector< tdk::io::ip::address >& addrs );
            
    tdk::io::ip::tcp::channel& channel( void );

    bool address_next( void );
    tdk::io::ip::address& address( void );
private:      
    tdk::io::ip::tcp::channel* _channel;
    std::vector< tdk::io::ip::address > _address;
    std::vector< tdk::io::ip::address >::iterator _address_it;
};
  
}}}}

#endif
