#ifndef __tdk_io_operation_h__
#define __tdk_io_operation_h__

namespace tdk {
namespace io {

class operation 
#if defined (_WIN32)
	: public OVERLAPPED 
#endif
{
public:
	typedef void (__stdcall* callback )( operation* );
public:
	operation( callback cb );
	~operation( void );

	void operator()( void );
private:
	callback _callback;
};

}}


#endif