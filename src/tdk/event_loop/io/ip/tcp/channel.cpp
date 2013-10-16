/*
 * channel.cpp
 *
 *  Created on: 2013. 10. 8.
 *      Author: tk
 */

#include <tdk/event_loop/io/ip/tcp/channel.hpp>
#include <tdk/event_loop/event_loop.hpp>
#include <tdk/error/error_platform.hpp>
#include <tdk/error/error_tdk.hpp>
#include <sys/uio.h>

namespace tdk {
namespace io{
namespace ip{
namespace tcp {

template < typename T_task >
class channel_forward_task : public tdk::task {
public:
	typedef void (channel::*handler)( T_task* t );

	channel_forward_task( channel* chan , handler h , T_task* t )
		: tdk::task( &channel_forward_task::on_task , t )
		, _channel( chan )
		, _handler( h ) {
	}

	~channel_forward_task( void ) {
	}

	void forward( void ) {
		(_channel->*_handler)( static_cast<T_task*>(context()));
	}

	static void on_task( tdk::task* t ) {
		channel_forward_task< T_task >* task_impl =
				static_cast<channel_forward_task< T_task >* >(t);
		task_impl->forward();
		delete task_impl;
	}
private:
	channel* _channel;
	handler _handler;
};

int channel::write_try_count = 4;

channel::channel( tdk::event_loop& loop )
	: _loop( &loop )
	, _channel_task( this , &channel::handle_io_event )
	, _complete_tasks(nullptr)
{

}

channel::~channel( void ) {

}

bool channel::accept( tdk::io::ip::tcp::accept_task* t ) {
	if ( t->accepted() == -1 )
		return false;
	_socket.handle(t->accepted());
	tdk::io::ip::socket::option::non_blocking nb;
	if ( _socket.set_option(nb))
		return true;
	_socket.handle(-1);
	return false;
}

void channel::connect( const std::vector<tdk::io::ip::address>& addrs
		, tdk::io::ip::tcp::connect_task* ct )
{
	ct->channel(this);
	ct->address(addrs);
	ct->io_bytes(0);
	ct->error(std::error_code());
	if ( _loop->in_loop() ) {
		connect_impl(ct);
	} else {
		tdk::task* ftask( new channel_forward_task< tdk::io::ip::tcp::connect_task>(
					this , &channel::connect_impl , ct));
		_loop->execute(ftask);
	}
}

void channel::connect_impl( tdk::io::ip::tcp::connect_task* ct ) {
	while ( !ct->address_is_eof() ) {
		tdk::io::ip::address& addr = ct->address_value();
		_socket.close();
		if ( _socket.open_tcp(addr.family())){
			tdk::io::ip::socket::option::non_blocking nb;
			if ( _socket.set_option( nb )){
				int r = 0;
				do {
					r = ::connect(_socket.handle()
							, addr.sockaddr()
							, addr.sockaddr_length());
				}while((r==-1)&&(errno==EINTR));
				if (( r == 0 ) || errno == EINPROGRESS ) {
					_write_tasks.add_front(ct);
					_channel_task.set_memfn( this , &channel::handle_connect_event );
					_channel_task.evt( EPOLLOUT | EPOLLONESHOT );
					if (_loop->io_impl().register_handle(
							_socket.handle()
							, &_channel_task ))
					{
						_loop->add_active();
						return;
					}
				}
			}
		}
		ct->address_next();
	}
	_socket.close();
	// recover
	_channel_task.set_memfn( this , &channel::handle_io_event );
	ct->error(tdk::platform::error());
	ct->io_bytes(0);
	_loop->execute(ct);
}

void channel::handle_io_event( int evt ) {
	tdk::slist_queue< tdk::task > queue;
	_complete_tasks = &queue;
	if ( evt & EPOLLERR ) {
		std::error_code ec = tdk::tdk_epoll_error;
		tdk::io::ip::socket::option::error sock_error;
		if ( _socket.get_option( sock_error ) && sock_error.value()!= 0){
			ec = tdk::platform::error( sock_error.value());
		}
		handle_error( ec , queue );
	} else {
		if ( evt & EPOLLHUP ) {
			handle_error( tdk::tdk_epoll_hang_up , queue );
		} else {
			if ( evt & EPOLLIN ) {
				handle_readable();
			}
			if ( evt & EPOLLOUT ) {
				handle_writeable();
			}
		}
	}

	while ( !_complete_tasks ->is_empty() ) {
		tdk::task* t = _complete_tasks->front();
		_complete_tasks->pop_front();
		_loop->remove_active();
		(*t)();
	}
	_complete_tasks = nullptr;
	if ( !register_handle() ){
		std::error_code ec = tdk::epoll_error(errno);
		handle_error( ec , queue );
		while ( !queue.is_empty() ) {
			tdk::task* t = queue.front();
			queue.pop_front();
			_loop->remove_active();
			(*t)();
		}
	}
}

void channel::handle_connect_event( int evt ) {
	tdk::io::ip::tcp::connect_task* ct =
			static_cast<tdk::io::ip::tcp::connect_task*>(
						_write_tasks.front()
					);
	_write_tasks.pop_front();
	if ( evt & EPOLLOUT ) {
		ct->error(std::error_code());
		tdk::io::ip::socket::option::error err;
		if ( _socket.get_option(err) && err.value()!= 0 ){
			ct->error(tdk::platform::error( err.value()));
		}
	} else {
		if ( evt & EPOLLERR ) {
			ct->error( tdk::tdk_epoll_error );
			tdk::io::ip::socket::option::error err;
			if ( _socket.get_option(err) && err.value()!= 0 ){
				ct->error(tdk::platform::error( err.value()));
			}
		} else if ( evt & EPOLLHUP ) {
			ct->error( tdk::tdk_epoll_hang_up );
		} else {
			ct->error( tdk::tdk_invalid_event );
		}
	}

	if ( ct->error() && ct->address_next()) {
		connect_impl( ct );
	} else {
		// recover to io_handler
		_channel_task.set_memfn( this , &channel::handle_io_event );
		_loop->remove_active();
		(*ct)();
	}
}

void channel::read( void* p , int sz , tdk::io::ip::tcp::read_task* rt ) {
	tdk::io::buffer_adapter buf(p,sz);
	read( buf , rt );
}

void channel::read( const tdk::io::buffer_adapter& buf
		, tdk::io::ip::tcp::read_task* rt ) {
	rt->channel(this);
	rt->buffers(buf);
	rt->io_bytes(0);
	rt->error(std::error_code());
	if ( _loop->in_loop() ) {
		read_impl(rt);
	} else {
		tdk::task* ftask( new channel_forward_task< tdk::io::ip::tcp::read_task>(
							this , &channel::read_impl , rt));
		_loop->execute(ftask);
	}
}

void channel::read_impl( tdk::io::ip::tcp::read_task* rt ) {
	if ( _socket.handle() == INVALID_SOCKET ){
		rt->error(tdk::tdk_invalid_call);
		_execute_task( rt );
		return;
	}

	// 이전에 요청된 read 가 없으며 handle_io_event 호출 흐름상에 없을시
	// 직접 등록
	if ( _read_tasks.is_empty() && _complete_tasks == nullptr ) {
		int event = EPOLLONESHOT | EPOLLIN ;
		if ( !_write_tasks.is_empty()){
			event |= EPOLLOUT;
		}
		_channel_task.evt( event );
		if (_loop->io_impl().register_handle(_socket.handle() , &_channel_task)){
			_read_tasks.add_tail(rt);
		} else {
			rt->error(tdk::platform::error());
			_execute_task( rt );
			return;
		}
	} else {
		_read_tasks.add_tail(rt);
	}
	_loop->add_active();
}

void channel::handle_readable( void ) {
	std::error_code error = std::error_code();
	while ( !_read_tasks.is_empty() ) {
		tdk::io::ip::tcp::read_task* rt =
				static_cast< tdk::io::ip::tcp::read_task* >( _read_tasks.front());
		int readsize = 0;
		do {
			readsize = readv( _socket.handle()
					, rt->buffers().buffers()
					, rt->buffers().count());
		} while (( readsize < 0) && ( errno == EINTR ));

		if ( readsize < 0 ) {
			if ((errno == EAGAIN) || (errno == EWOULDBLOCK ) ) {
			} else {
				error = tdk::platform::error();
			}
			break;
		} else if ( (readsize == 0) && rt->buffers().size() != 0 ){
			error = tdk::tdk_network_remote_closed;
			break;
		} else {
			_read_tasks.pop_front();
			rt->io_bytes( readsize );
			_complete_tasks->add_tail( rt );
		}
	}
	if ( error )  {
		handle_error( error , *_complete_tasks );
	}
}

void channel::write( void* p , int sz , tdk::io::ip::tcp::write_task* wt ) {
	tdk::io::buffer_adapter buf(p,sz);
	write( buf , wt );
}

void channel::write( const tdk::io::buffer_adapter& buf
		, tdk::io::ip::tcp::write_task* wt ) {
	wt->channel(this);
	wt->buffers(buf);
	wt->io_bytes(0);
	wt->error(std::error_code());
	if ( _loop->in_loop() ) {
		write_impl(wt);
	} else {
		tdk::task* ftask(
				new channel_forward_task< tdk::io::ip::tcp::write_task>(
									this , &channel::write_impl , wt));
		_loop->execute(ftask);
	}
}

void channel::write_impl( tdk::io::ip::tcp::write_task* wt ) {
	if ( _socket.handle() == INVALID_SOCKET ){
		wt->error(tdk::tdk_invalid_call);
		_execute_task( wt );
		return;
	}

	if ( !_write_tasks.is_empty()){
		_write_tasks.add_tail(wt);
		return;
	}

	for ( int i = 0 ; i < channel::write_try_count ; ++i ) {
		int writesize = 0;
		tdk::io::buffer_adapter remain_buffer = wt->remain_buffers();
		do {
			writesize = writev( _socket.handle()
				, remain_buffer.buffers()
				, remain_buffer.count());
		} while (( writesize < 0)  && ( errno == EINTR ));
		if ( writesize < 0 ) {
			if ( errno == EAGAIN || errno == EWOULDBLOCK ) {
				break;
			} else {
				wt->error( tdk::platform::error());
				_execute_task( wt );
				return;
			}
		} else {
			int write_before = wt->io_bytes();
			int total_write = wt->buffers().size();
			int writed = write_before + writesize;
			wt->io_bytes( writed );
			if ( writed == total_write ) {
				_execute_task( wt );
				return;
			}
		}
	}

	if ( _complete_tasks == nullptr ){
		int event = EPOLLONESHOT | EPOLLOUT ;
		if ( !_read_tasks.is_empty()){
			event |= EPOLLIN;
		}
		_channel_task.evt( event );
		if (_loop->io_impl().register_handle(_socket.handle() , &_channel_task)){
			_write_tasks.add_tail(wt);
		} else {
			wt->error(tdk::platform::error());
			_loop->execute(wt);
			return;
		}
	} else {
		_write_tasks.add_tail(wt);
	}
	_loop->add_active();
}

void channel::handle_writeable( void ) {
	std::error_code error = std::error_code();
	bool cont = true;
	while ( cont && !_write_tasks.is_empty() ) {
		for ( int i = 0 ; i < channel::write_try_count ; ++i ) {
			tdk::io::ip::tcp::write_task* wt =
					static_cast< tdk::io::ip::tcp::write_task* >( _write_tasks.front());
			int writesize = 0;
			tdk::io::buffer_adapter remain = wt->remain_buffers();
			do {
				writesize = writev( _socket.handle()
					, remain.buffers()
					, remain.count());
			} while (( writesize < 0)  && ( errno == EINTR ));
			if ( writesize < 0 ) {
				if ( errno == EAGAIN || errno == EWOULDBLOCK ) {
				} else {
					error = tdk::platform::error();
				}
				cont = false;
				break;
			} else {
				int write_before = wt->io_bytes();
				int total_write = wt->buffers().size();
				int writed = write_before + writesize;
				wt->io_bytes( writed );
				if ( writed == total_write ) {
					_write_tasks.pop_front();
					_complete_tasks->add_tail(wt);
				}
			}
		}
	}
	if ( error )  {
		handle_error( error , *_complete_tasks );
	}
}

bool channel::register_handle( void ) {
	int event = EPOLLONESHOT;
	if ( !_read_tasks.is_empty()) {
		event |= EPOLLIN;
	}
	if ( !_write_tasks.is_empty()){
		event |= EPOLLOUT;
	}
	if ( event == EPOLLONESHOT )
		return true;

	_channel_task.evt( event );
	return _loop->io_impl().register_handle(_socket.handle() , &_channel_task);
}

tdk::io::ip::socket& channel::socket(){
	return _socket;
}

void channel::handle_error( const std::error_code& ec
		, tdk::slist_queue<tdk::task>& queue ) {
	_socket.close();
	while ( !_read_tasks.is_empty() ) {
		tdk::io::task* rt =
				static_cast< tdk::io::task* >(
						_read_tasks.front());
		_read_tasks.pop_front();
		rt->error( ec );
		queue.add_tail( rt );
	}
	while( !_write_tasks.is_empty()) {
		tdk::io::task* ct =
				static_cast< tdk::io::task* >(
						_write_tasks.front());
		_write_tasks.pop_front();
		ct->error( ec );
		queue.add_tail(ct);
	}
}

void channel::close( tdk::io::ip::tcp::close_task* ct ) {
	ct->channel(this);
	_loop->execute(ct->internal_task());
}

void channel::close_impl( tdk::io::ip::tcp::close_task* ct ) {
	tdk::slist_queue< tdk::task > queue;
	handle_error( tdk::tdk_user_abort , queue );
	while ( !queue.is_empty() ) {
		tdk::task* t = queue.front();
		queue.pop_front();
		_loop->remove_active();
		(*t)();
	}
	(*ct)();
}

void channel::_execute_task( tdk::task* t ) {
	if ( _complete_tasks != nullptr ) {
		_complete_tasks->add_tail(t);
		_loop->add_active();
	} else {
		_loop->execute(t);
	}
}

tdk::event_loop& channel::loop( void ) {
	return *_loop;
}

/*
static void channel_connect_handler( tdk::task* t ) {
	channel* ch = static_cast< channel* >( t->context());
	ch->handle_connect_event();
}

static void channel_io_handler( tdk::task* t ) {
	channel* ch = static_cast< channel* >( t->context());
	ch->handle_io_event();
}
*/
}}}}

