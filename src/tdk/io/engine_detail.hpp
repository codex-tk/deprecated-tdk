#ifndef __tdk_io_engine_detail_h__
#define __tdk_io_engine_detail_h__
#if defined ( _WIN32 ) 
#include <tdk/io/completion_port_win32.hpp>
#elif defined( linux ) || defined ( __linux )

#elif defined ( __MACOSX__ ) || defined ( __APPLE__ ) 

#else

#endif

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
}}

#endif
