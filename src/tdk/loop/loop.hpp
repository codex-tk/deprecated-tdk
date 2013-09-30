#ifndef __tdk_loop_h__
#define __tdk_loop_h__

#include <atomic>
#include <tdk/util/list_node.hpp>
#include <tdk/threading/spin_lock.hpp>

#if defined ( linux ) || defined( __linux )
#include <tdk/loop/epoll.hpp>
#endif

#include <tdk/loop/timer_scheduler.hpp>

namespace tdk {

class loop {
public:    
#if defined ( linux ) || defined( __linux )
    typedef tdk::epoll impl_type;
#endif
public:
    loop( void );
    ~loop( void );

    impl_type& implementation( void );
    tdk::timer_scheduler& scheduler( void );

    void add_active( void );
    void remove_active( void );

    void run( void );

    void execute( tdk::req_handle* req );

    template < typename T_handler >
    void execute( const T_handler& handler );
        
    static loop& default_loop( void );
    static loop* current_loop( void );
private:
    void _run_reqs( void );
private:        
    impl_type _impl;
    std::atomic<int> _active_handles;    
    tdk::threading::spin_lock _lock;
    tdk::slist_queue< tdk::req_handle > _req_queue;
    tdk::timer_scheduler _scheduler;
};

template < typename T_handler >
void loop::execute( const T_handler& handler ){
    class work {
    public:
        work( const T_handler& h ) 
            : _request( &work::_on_execute , this )
            , _handler( h ){ }
        ~work( void ){ }
        void execute( void ){ _handler(); }

        static void _on_execute( req_handle* req ) {
            work* w = static_cast< work* >( req->data() );
            w->execute();
            delete w;
        }

        tdk::req_handle* request( void ) {
            return &_request;
        }
    private:
        tdk::req_handle _request;
        T_handler _handler;
    };
    work* w = new work( handler );
    execute( w->request());
}

}

#endif
