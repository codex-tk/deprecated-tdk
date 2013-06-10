#ifndef __tdk_io_handler_h__
#define __tdk_io_handler_h__

#include <tdk/task/handler.hpp>
#include <tdk/error/error_code.hpp>

namespace tdk {
namespace io{

class handler : public OVERLAPPED {
public:
	handler( void );
	virtual ~handler( void );
	virtual void operator()(const tdk::error_code& e , int iobytes) = 0;
};

}}

#endif