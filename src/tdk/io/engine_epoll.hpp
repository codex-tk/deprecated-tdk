#ifndef __tdk_io_engine_epoll_h__
#define __tdk_io_engine_epoll_h__

#include <tdk/tdk.hpp>
#include <tdk/time/date_time.hpp>
#include <tdk/io/eventfd.hpp>
#include <tdk/io/engine_detail.hpp>

namespace tdk {
namespace io{

class engine {
public:
    struct context{
        void* object;
        void (*callback)(void* obj , int event);
        void operator()( int evt ){
            if ( callback != nullptr  ) 
                (*callback)( object , evt );
        }
    };
public:
    engine( void );
    ~engine( void );

    void wake_up( void );

    bool ctl( int op ,  int fd , int ev , void* ptr );

    void post( tdk::io::operation* op , const std::error_code& ec );   
    void drain(void);


private:
    tdk::io::operation* fetch( void );
public:
    template < typename T_handler >
    void post( const T_handler& h ) {
        class op_base : public tdk::io::operation {
        public:
            op_base( tdk::io::operation::callback cb )
                : operation( cb ){}
            bool end_operation( void ){ return true; }
        };
        typedef tdk::io::detail::operation_impl<
            T_handler , op_base , tdk::io::detail::dispatcher_none_param 
            > handler_op;
        handler_op* op = new handler_op( h );
        post( op , std::error_code());
    }
    int wait( const tdk::time_span& ts );
private:
    int _epoll_fd;
    tdk::io::eventfd _op_event;
    tdk::threading::spin_lock _op_queue_lock;
    tdk::slist_queue< tdk::io::operation > _op_queue;
    engine::context _drain_context;
};


}}


#endif
