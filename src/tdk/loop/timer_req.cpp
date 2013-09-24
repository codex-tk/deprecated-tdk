#include <tdk/loop/timer_req.hpp>

namespace tdk {

timer_req::timer_req( const tdk::date_time& t ) 
    : _expired_at( t ){

}

timer_req::~timer_req( void )
{

}

const tdk::date_time& timer_req::expired_at( void ) const {
    return _expired_at;
}

void timer_req::expired_at( const tdk::date_time& t ) {
    _expired_at = t;
}


}

