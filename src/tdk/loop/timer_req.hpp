#ifndef __tdk_timer_req_h__
#define __tdk_timer_req_h__

#include <tdk/time/date_time.hpp>
#include <tdk/loop/req_base.hpp>

namespace tdk {

class timer_req : public tdk::io_req_handle {
public:
    timer_req( const tdk::date_time& t = tdk::date_time::utc());
    ~timer_req( void );

    const tdk::date_time& expired_at( void ) const;
    void expired_at( const tdk::date_time& t );
private:
    tdk::date_time _expired_at;
};

}


#endif
