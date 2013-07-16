#include <tdk/io/engine_epoll.hpp>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <tdk/error/error_platform.hpp>
#include <tdk/io/ip/tcp/operation/connect_operation_epoll.hpp>
#include <tdk/io/ip/tcp/socket_epoll.hpp>

namespace tdk {
namespace io{
namespace detail{

static void drain_callback( void* obj , int event ) {
    tdk::io::engine* e = static_cast< tdk::io::engine* >(obj);
    e->drain();
}

}

engine::engine( void ){
    _epoll_fd = epoll_create( 1 );
    if ( _epoll_fd == -1 ) 
        throw std::system_error( tdk::platform::error());

    _drain_context.object = this;
    _drain_context.callback = &detail::drain_callback;

    if ( !ctl( EPOLL_CTL_ADD 
        , _op_event.handle() 
        , EPOLLIN | EPOLLET | EPOLLONESHOT 
        , &_drain_context ))
    {
        throw std::system_error( tdk::platform::error());
    }
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

void engine::wake_up( void ) {
    ctl( EPOLL_CTL_MOD 
        , _op_event.handle() 
        , EPOLLIN | EPOLLET | EPOLLONESHOT 
        , &_drain_context );
    _op_event.set();
}

void engine::post( tdk::io::operation* op 
        , const std::error_code& ec  ) 
{
    tdk::threading::scoped_lock<> guard( _op_queue_lock );
    op->error( ec );
    _op_queue.add_tail( op );
    wake_up();
}

tdk::io::operation* engine::fetch( void ) {
    tdk::threading::scoped_lock<> guard( _op_queue_lock);
    if ( _op_queue.is_empty() ) {
        return nullptr;
    }
    tdk::io::operation* op = _op_queue.front();
    _op_queue.pop_front();
    return op;
}

void engine::drain( void ) {
    while( true ){
        tdk::io::operation* op = fetch();
        if ( op ) {
            (*op)( op->error() , op->io_bytes()  );
        } else {
            break;
        }
    }
    _op_event.reset();
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
/*
void engine::async_connect( tdk::io::ip::tcp::connect_operation* op ){
    if ( !op->socket().open_tcp( op->address().family())){
        post( op , tdk::platform::error());
        return;
    }
    tdk::io::ip::socket::option::non_blocking nb;
    if ( !op->socket().set_option( nb )){
        post( op , tdk::platform::error());
        return;
    }    
    if ( !op->socket().connect( op->address())){
        if ( errno != EINPROGRESS ) {
            post( op , tdk::platform::error( errno ));    
            return;
        }
    }
    if ( !ctl( EPOLL_CTL_ADD 
            , op->socket().handle() 
            , EPOLLOUT | EPOLLET | EPOLLONESHOT 
            , static_cast<void*>(op->context())))
    {
        post( op , tdk::platform::error());
        return;
    }
}
*/
}}


