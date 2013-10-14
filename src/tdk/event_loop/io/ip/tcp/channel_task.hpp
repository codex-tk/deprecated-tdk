/*
 * channel_task.hpp
 *
 *  Created on: 2013. 10. 10.
 *      Author: tk
 */

#ifndef CHANNEL_TASK_HPP_
#define CHANNEL_TASK_HPP_

#include <tdk/event_loop/io/task.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {
class channel;

class channel_task: public tdk::io::task {
public:
	channel_task( void );
	channel_task( tdk::task::handler h , void* ctx );
	~channel_task();

	tdk::io::ip::tcp::channel* channel( void );
	void channel( tdk::io::ip::tcp::channel* chan );
private:
	tdk::io::ip::tcp::channel* _channel;
};

/*
template < typename Arg >
class channel_task;

template < typename Arg >
class channel_thread_task : public tdk::task {
public:
	typedef void (channel::*handler)( Arg* );

	channel_thread_task( Arg* task )
		: tdk::task( channel_thread_task::on_task , task )
		, _handler( nullptr )
	{
	}

	~channel_thread_task( void ) {

	}

	void set_channel_handler(  handler h ) {
		_handler = h;
	}

	void forward(void) {
		Arg* a = static_cast< Arg* >(context());
		if ( _handler ) {
			((*(a->channel())).*_handler)( a );
		}
	}

	static void on_task( tdk::task* t ) {
		channel_thread_task* ctt = static_cast< channel_thread_task* >(t);
		ctt->forward();
	}
private:
	handler _handler;
};


void execute_impl( tdk::io::ip::tcp::channel* chan , tdk::task*);


template < typename Arg >
class channel_task: public tdk::io::task {
public:
	channel_task( Arg* a )
		: _thread_task(a)
		,_channel(nullptr){
	}

	channel_task( Arg* a , tdk::task::handler h , void* ctx )
	: tdk::io::task( h , ctx )
		, _thread_task(a)
		, _channel( nullptr )
	{
	}

	~channel_task() {
	}

	channel_thread_task<Arg>* thread_task( void ) {
		return &_thread_task;
	}

	tdk::io::ip::tcp::channel* channel( void ) {
		return _channel;
	}

	void channel( tdk::io::ip::tcp::channel* chan ) {
		_channel = chan;
	}

	void execute_thread_task( typename channel_thread_task<Arg>::handler h ) {
		_thread_task.set_channel_handler(h);
		execute_impl( _channel , &_thread_task );
	}
private:
	channel_thread_task<Arg> _thread_task;
	tdk::io::ip::tcp::channel* _channel;
};*/

} /* namespace tcp */
} /* namespace ip */
} /* namespace io */
} /* namespace tdk */

#endif /* CHANNEL_TASK_HPP_ */
