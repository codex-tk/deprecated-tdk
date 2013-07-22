#include <tdk/io/timerfd.hpp>
#include <sys/timerfd.h>

namespace tdk {
namespace io {

timerfd::timerfd( void ) {
    _fd = timerfd_create( CLOCK_REALTIME , TFD_NONBLOCK );
}

timerfd::~timerfd( void ) {
    close(_fd);
}

void timerfd::set( const tdk::date_time& expired ) {
    itimerspec ts;
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;
    ts.it_value = tdk::time::tick::to_timespec( expired.time());
    timerfd_settime( _fd , TFD_TIMER_ABSTIME , &ts , nullptr );
}

void timerfd::reset( void ) {
    uint64_t val;
    while ( true ) {
        int ret = read( _fd , &val , sizeof(val));
        if ( ret == -1 ){
            return;        
        }
    }
}

int timerfd::handle(void){
    return _fd;
}

}}


