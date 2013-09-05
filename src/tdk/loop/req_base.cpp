#include <tdk/loop/req_base.hpp>

namespace tdk {
    
req_handle::req_handle( void ) 
   : _callback( nullptr )
   ,_data( nullptr )
{
}

req_handle::req_handle( 
    req_handle::callback_type cb
    , void* p ) 
   : _callback( cb )
   ,_data( p )
{
}

req_handle::~req_handle( void ) {

}

void*  req_handle::data( void ) {
    return _data;
}

void req_handle::set( callback_type cb , void* data ) {
    _callback = cb;
    _data = data;
}

void  req_handle::data( void* p ) {
    _data = p;
}

void  req_handle::callback( callback_type cb ) {
    _callback  = cb ;
}

void  req_handle::invoke( void ) {
    if ( _callback )
        _callback( this );
}

io_req_handle::io_req_handle( void ) 
    : _io_bytes(0){
}

io_req_handle::~io_req_handle( void ){
}

const std::error_code& io_req_handle::error( void ){
    return _error;
}

void io_req_handle::error( const std::error_code& ec ) {
    _error = ec;
}

int io_req_handle::io_bytes( void ){
    return _io_bytes;
}

void io_req_handle::io_bytes( int io ) {
    _io_bytes = io;
}
/*
req_base::req_base( void* d )
    : _data( d ){
}

void* req_base::data( void ){
    return _data;
}
void req_base::data( void* d ) {
    _data = d;
}

req_base::~req_base( void ){
}

io_req_base::io_req_base( void ) 
    : _io_bytes(0){
}

io_req_base::~io_req_base( void ){
}

const std::error_code& io_req_base::error( void ){
    return _error;
}

void io_req_base::error( const std::error_code& ec ) {
    _error = ec;
}

int io_req_base::io_bytes( void ){
    return _io_bytes;
}

void io_req_base::io_bytes( int io ) {
    _io_bytes = io;
}
*/

}


