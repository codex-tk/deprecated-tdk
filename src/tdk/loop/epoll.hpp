#ifndef __loop_epoll_h__
#define __loop_epoll_h__

#include <tdk/time/time_span.hpp>
#include <tdk/loop/req_base.hpp>

namespace tdk {

class epoll{
public:
    class event_handler : public tdk::req_handle {
    public:
        event_handler( void );
        ~event_handler( void );

        int  events( void );
        void events( int e );
    private:
        int _events;
    };

    class wake_up_impl : public epoll::event_handler {
    public:
        wake_up_impl( void );
        ~wake_up_impl( void );

        void wake_up( void );

        int handle( void );

        void handle_event( void );
    private:
        int _event_fd;
    };
    
    epoll( void );
    ~epoll( void );

    bool reg( int fd , int evt , event_handler* on_event );
    bool unreg( int fd );

    void reg_req( tdk::req_handle* req );

    // call by other thread
    void wake_up( void );

    void wait( const tdk::time_span& ts );
private:
    int _handle;
    epoll::wake_up_impl _wake_up;
    tdk::slist_queue< tdk::req_handle > _reg_reqs;
};

}
    
#endif
