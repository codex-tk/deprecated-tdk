#ifndef __tdk_req_base_h___
#define __tdk_req_base_h__

#include <system_error>
#include <tdk/util/list_node.hpp>

namespace tdk {

class req_handle 
    : public tdk::slist_node< req_handle >{
public:
    typedef void (*callback_type)( req_handle* req );
    
    req_handle( void );
    explicit req_handle( callback_type cb , void* p = nullptr );
    ~req_handle( void );

    void* data( void );

    void set( callback_type cb , void* data );

    void data( void* p );
    void callback( callback_type cb );

    void invoke( void );
private:
    callback_type _callback;
    void* _data;
};

class io_req_handle : public req_handle {
public:
    io_req_handle( void );
    ~io_req_handle( void );

    const std::error_code& error( void ) ;
    void error( const std::error_code& ec ) ;

    int io_bytes( void );
    void io_bytes( int io );
private:
    std::error_code _error;
    int _io_bytes;
};

}

#endif
