#ifndef __tdk_task_operation_h__
#define __tdk_task_operation_h__

#include <tdk/error/error_code.hpp>
#include <tdk/util/list_node.hpp>
namespace tdk {
namespace task {

#if defined (_WIN32)
typedef OVERLAPPED operation_base;
#else
typedef struct _operation_base {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union {
        struct {
            DWORD Offset;
            DWORD OffsetHigh;
        } DUMMYSTRUCTNAME;
        PVOID Pointer;
    } DUMMYUNIONNAME;
    HANDLE  hEvent;
} operation_base ;
#endif

class operation 
	: public operation_base 
	, public tdk::slist_node< operation >{
public:
	operation( void );
	virtual ~operation( void );
	virtual void operator()(void ) = 0;

	tdk::error_code error( void );
	void error( const tdk::error_code& code );
	int  io_byte( void );
	void io_byte( int );
	void* object( void );
	void  object( void* o );

	void reset( void );
};

}}

#endif