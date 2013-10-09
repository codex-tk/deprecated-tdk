#ifndef __tdk_io_epoll_h__
#define __tdk_io_epoll_h__

#include <tdk/event_loop/io_task.hpp>
#include <tdk/time/time_span.hpp>
#include <sys/epoll.h>

namespace tdk {
namespace io {

class epoll{
public:
	class task : public io_task {
	public:
		task( void );
		task( tdk::task::handler h , void* ctx = nullptr );
		~task( void );

		int evt( void );
		void evt( int e );

		epoll_event* impl( void );
	private:
		epoll_event _evt;
	};

	class event_fd_task : public epoll::task {
	public:
		event_fd_task( void );
		~event_fd_task( void );

		int handle( void );

		void set( void );
	private:
		int _event_fd;
	};

	epoll( void );
	~epoll( void );

	bool register_handle( int fd , epoll::task* task );
	void unregister_handle( int fd );

	void wake_up( void );

	void wait( const tdk::time_span& w );
private:
	int _epoll_fd;
	event_fd_task _wake_up_task;
};

}
}


#endif
