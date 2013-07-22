#ifndef __tdk_task_thread_timer_h__
#define __tdk_task_thread_timer_h__

#include <tdk/threading/spin_lock.hpp>
#include <thread>

namespace tdk {
namespace task {

class thread_timer {
public:
    thread_timer( void );
    ~thread_timer( void );

    void on_timer( void );
private:
    tdk::threading::spin_lock _lock;
    std::thread _thread;
};

}}

#endif
