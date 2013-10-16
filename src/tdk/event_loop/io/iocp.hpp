#ifndef __tdk_io_iocp_h__
#define __tdk_io_iocp_h__

namespace tdk {
namespace io {

class iocp {
public:
	class task : public io::task {
	public:
		task( void );
		task( tdk::task::handler h , void* ctx = nullptr );
		~task( void );

		OVERLAPPED* impl( void );
	private:
		struct overlapped_ex : public OVERLAPPED {
			iocp::task* task;
		};
		overlapped_ex _ov;
	};

	iocp( void );
	~iocp( void );

	bool register_handle( SOCKET fd , void* );
	void unregister_handle( SOCKET fd );

	void wake_up( void );

	void wait( const tdk::time_span& w );
private:
	HANDLE _iocp;
};

}}

#endif