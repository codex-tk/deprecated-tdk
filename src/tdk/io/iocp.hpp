#ifndef __tdk_io_iocp_h__
#define __tdk_io_iocp_h__

#if defined ( _WIN32 )

#include <tdk/io/task.hpp>
#include <tdk/time/time_span.hpp>
#include <atomic>

namespace tdk {
namespace io {

class iocp {
public:
	iocp( void );
	~iocp( void );

	bool register_handle( SOCKET fd , void* );
	void unregister_handle( SOCKET fd );

	void wake_up( void );

	void wait( const tdk::time_span& w );
private:
	HANDLE _iocp;
	std::atomic<int> _posted;
};

}}

#endif
#endif
