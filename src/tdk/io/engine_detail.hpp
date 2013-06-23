#ifndef __tdk_io_engine_detail_h__
#define __tdk_io_engine_detail_h__

#include <tdk/io/completion_port_win32.hpp>
#include <tdk/io/operation.hpp>
#include <tdk/threading/spin_lock.hpp>
#include <tdk/util/list_node.hpp>
namespace tdk {
namespace io {
namespace ip{
namespace tcp{

class connect_operation;
class send_operation;
class recv_operation;
class accept_operation;

}}

namespace detail {

class drain_operation;

template < typename T_engine , typename T_op > 
void error_handler( T_engine* e , T_op* op ) {
	op->socket().close();
	op->error( tdk::platform::error() );
	e->_post( op );
}

}}}

#endif