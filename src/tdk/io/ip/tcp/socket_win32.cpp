#include "stdafx.h"
#include <tdk/io/ip/tcp/socket_win32.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

socket::socket( tdk::io::engine& engine )
	: _engine(engine)
{

}

socket::~socket( void ) {

}

}}}}
