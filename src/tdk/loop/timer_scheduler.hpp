#ifndef __tdk_loop_timer_scheduler_h__
#define __tdk_loop_timer_scheduler_h__

#include <tdk/loop/timer_req.hpp>

#include <list>

namespace tdk {

class timer_scheduler{
public:
    timer_scheduler( void );
    ~timer_scheduler( void );

    void schedule( tdk::timer_req* req );
    void cancel( tdk::timer_req* req );
    void drain( void );
    tdk::date_time wakeup_at( void );
private:
    std::list< tdk::timer_req* > _reqs;
    std::list< tdk::timer_req* > _cancels;
};

}

#endif
