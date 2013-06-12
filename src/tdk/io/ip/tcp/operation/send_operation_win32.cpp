#include "stdafx.h"
#include <tdk/io/ip/tcp/operation/send_operation_win32.hpp>
#include <tdk/io/ip/tcp/socket_win32.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

send_operation::send_operation( tdk::io::ip::tcp::socket& fd
    , const tdk::io::buffer_adapter& buffer
    , tdk::io::operation::callback cb )
    : tdk::io::operation( cb )
    , _socket( &fd )
    , _buffer_adapter( buffer )
{
    
}

send_operation::~send_operation( void ) {

}

bool send_operation::end_opearation( void ) {
    return true;
}

void send_operation::process( void ) {
    _socket->engine().async_send( this );
}

tdk::io::ip::tcp::socket& send_operation::socket( void ) {
    return *_socket;
}

const tdk::io::buffer_adapter& send_operation::buffer_adapter( void ) {
    return _buffer_adapter;
}

}}}}