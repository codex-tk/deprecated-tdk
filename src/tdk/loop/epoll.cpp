#include <tdk/loop/epoll.hpp>
#include <sys/epoll.h>
#include <unistd.h>

namespace tdk {

epoll::epoll( void )
    : _handle(epoll_create(4096))
{
    reg( _wake_up.handle() , EPOLLIN , &_wake_up );
}

epoll::~epoll( void ){
    close( _handle );
}

bool epoll::reg( int fd , int evt , epoll::event_handler* h ) {
    epoll_event reg_evt;
    reg_evt.events = evt;
    reg_evt.data.ptr = h;
    if ( epoll_ctl( _handle , EPOLL_CTL_MOD , fd , &reg_evt ) == 0 )
        return true;
    if ( errno == ENOENT ) 
        return epoll_ctl( _handle , EPOLL_CTL_ADD , fd , &reg_evt ) == 0;
    return false;
}

bool epoll::unreg( int fd ) {
   if ( epoll_ctl( _handle , EPOLL_CTL_DEL , fd , nullptr ) == 0 )
        return true;
    if ( errno == ENOENT ) 
        return true;
    return false;
}

void epoll::wake_up( void ) {
    _wake_up.wake_up();
}

void epoll::reg_req( tdk::req_handle* req ) {
    _reg_reqs.add_tail( req );
}

void epoll::wait( const tdk::time_span& ts ) {
    while ( !_reg_reqs.is_empty()){
        tdk::req_handle* req = _reg_reqs.front();
        _reg_reqs.pop_front();
        req->invoke();
    }
    static const int k_max_events = 256;
    struct epoll_event events[k_max_events];
    int nev  = epoll_wait( _handle
            , events
            , k_max_events
            , ts.total_milli_seconds());

    if ( nev == -1 ) {
        if ( errno != EINTR ) {
            return;
        }
    } else {
        for ( int i = 0 ; i < nev; ++i ){
            epoll::event_handler* handler = 
                static_cast< epoll::event_handler* >( events[i].data.ptr );
            if ( handler ) {
                handler->events( events[i].events );
                handler->invoke();
            }
        }
    }
}


epoll::event_handler::event_handler( void ) 
    :_events(0)
{
}

epoll::event_handler::~event_handler( void ) {

}

int epoll::event_handler::events( void ) {
    return _events;
}

void epoll::event_handler::events( int e ) {
    _events = e;
}


#include <sys/eventfd.h>

static void _on_wake_up( tdk::req_handle* req ) {
    epoll::wake_up_impl* impl = static_cast< epoll::wake_up_impl* >( req );
    impl->handle_event();
}

epoll::wake_up_impl::wake_up_impl( void ) 
    : _event_fd( eventfd( 0 , EFD_NONBLOCK ))
{
    set( &_on_wake_up , NULL );
}

epoll::wake_up_impl::~wake_up_impl( void ){
    close( _event_fd );
}

int epoll::wake_up_impl::handle( void ){
    return _event_fd;
}

void epoll::wake_up_impl::wake_up( void ) {
    eventfd_write( _event_fd , 1 );    
}

void epoll::wake_up_impl::handle_event( void ) {
    eventfd_t val = 0;
    eventfd_read( _event_fd , &val );
}

}
