#ifndef __timerfd_h__
#define __timerfd_h__

#include <tdk/time/date_time.hpp>

namespace tdk {
namespace io {

class timerfd {
public:
    timerfd( void );
    ~timerfd( void );
    
    void set( const tdk::date_time& expired );
    void reset( void );        

    int handle( void );

private:
    int _fd;
};
}}

#endif
