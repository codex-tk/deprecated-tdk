#include <tdk/io/engine_epoll.hpp>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <tdk/error/error_platform.hpp>
#include <tdk/io/ip/tcp/operation/connect_operation_epoll.hpp>
#include <tdk/io/ip/tcp/socket_epoll.hpp>

namespace tdk {
namespace io{
    
engine::engine( void )
{
    _posted.store(0);
    _epoll_fd = epoll_create( 1 );
    if ( _epoll_fd == -1 ) 
        throw std::system_error( tdk::platform::error());
    _scheduler = new scheduler( *this ); 
}

engine::~engine( void ) {
    close( _epoll_fd );
}

bool engine::ctl( int op ,  int fd , int ev , void* ptr ) {
    epoll_event evt;
    evt.events = ev;
    evt.data.ptr = ptr;
    return epoll_ctl( _epoll_fd , op  , fd , &evt )==0;
}
void engine::post( tdk::io::operation* op 
        , const std::error_code& ec  ) 
{
    op->error(ec);
    _scheduler->post( op );
}

int engine::wait( const tdk::time_span& ts ){
    static const int k_max_events = 256;
    struct epoll_event events[k_max_events];
    int nev  = epoll_wait( _epoll_fd 
            , events
            , k_max_events
            , ts.total_milli_seconds());

    if ( nev == -1 ) {
        std::error_code ec = tdk::platform::error();
    } else {
        for ( int i = 0 ; i < nev; ++i ){
            epoll_event* ev = events + i;
            engine::context* ctx = 
                static_cast< engine::context* >( ev->data.ptr );
            if ( ctx ) {
                (*ctx)(ev->events);    
            }
        }
    }
    return nev;
}
void engine::inc_posted( void ) {
    _posted.fetch_add(1);
}
void engine::dec_posted( void ) {
    _posted.fetch_sub(1);
}

void engine::run( void ) {
    while ( _posted.load() > 0 ) {
        wait( tdk::time_span::infinite());
    }
}

engine::timer_id engine::schedule( engine::timer_id& id ){
    _scheduler->schedule( id );
    return id;
}

void engine::cancel( engine::timer_id& id ){
    _scheduler->cancel( id );
}

}}


