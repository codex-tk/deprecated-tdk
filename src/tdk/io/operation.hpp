#ifndef __tdk_io_operation_h__
#define __tdk_io_operation_h__

#include <tdk/tdk.hpp>
#include <tdk/util/list_node.hpp>

namespace tdk {
namespace io {

class operation 
#if defined (_WIN32)
	: public OVERLAPPED 
    , public tdk::slist_node< operation >
#else
    : public tdk::slist_node< operation >
#endif
{
public:
	typedef void (__stdcall* callback )( operation* );
public:
	operation( callback cb );
	~operation( void );

	void operator()( const std::error_code& e , int io_bytes );

	std::error_code error( void );
	int io_bytes( void );

	void error( const std::error_code& ec );

	void reset( void );
private:
	callback _callback;
#if defined( _WIN32 )

#else
    std::error_code _error;
    int _io_bytes;
#endif    
};


}}

#include <tdk/io/operation_impl.hpp>

#endif
