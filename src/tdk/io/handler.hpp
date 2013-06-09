#ifndef __tdk_io_handler_h__
#define __tdk_io_handler_h__

#include <tdk/task/handler.hpp>
#include <tdk/error/error_code.hpp>

namespace tdk {
namespace io{

class handler {
public:
	handler( void ){}
	virtual ~handler( void ){}
	virtual void operator()(void) = 0;
private:
};

typedef tdk::task::handler< void ( const tdk::error_code& e , int iobytes ) > handler_type;

}}

#endif