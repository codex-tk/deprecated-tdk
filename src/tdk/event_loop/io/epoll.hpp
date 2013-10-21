#ifndef __tdk_io_epoll_h__
#define __tdk_io_epoll_h__

#include <tdk/event_loop/io/task.hpp>
#include <tdk/time/time_span.hpp>
#include <sys/epoll.h>

namespace tdk {
namespace io {

class epoll{
public:
	/*
	class task : public io::task {
	public:
		task( void );
		task( tdk::task::handler h , void* ctx = nullptr );
		~task( void );

		int evt( void );
		void evt( int e );

		epoll_event* impl( void );
	private:
		epoll_event _evt;
	};*/


	class event_fd_task : public io::task {
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

	bool register_handle( int fd , io::task* task );
	void unregister_handle( int fd );

	void wake_up( void );

	void wait( const tdk::time_span& w );
private:
	int _epoll_fd;
	event_fd_task _wake_up_task;
public:
	template < typename T , typename R >
	class memfn_task : public io::task {
	public:
		typedef R (T::*handler)( int evt );
		memfn_task( T* obj , memfn_task::handler h )
			: io::task( memfn_task::on_task , obj )
			, _handler(h)
		{

		}

		void set_memfn( T* obj , memfn_task::handler h ) {
			_handler = h;
			set_handler( memfn_task::on_task , obj );
		}

		void invoke( void ) {
			T* obj = static_cast< T* >( context());
			int evts = evt();
			((*obj).*_handler)(evts);
		}

		static void on_task( tdk::task* t ) {
			epoll::memfn_task<T,R>* tmf = static_cast< epoll::memfn_task<T,R>* >(t);
			tmf->invoke();
		}
	private:
		handler _handler;
	};
};

}
}


#endif
