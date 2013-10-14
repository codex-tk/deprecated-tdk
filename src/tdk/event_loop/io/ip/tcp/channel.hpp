/*
 * channel.hpp
 *
 *  Created on: 2013. 10. 8.
 *      Author: tk
 */

#ifndef _tdk_io_ip_tcp_channel_h__
#define _tdk_io_ip_tcp_channel_h__

#include <tdk/io/ip/socket.hpp>
#include <tdk/event_loop/io/epoll.hpp>
#include <tdk/event_loop/io/ip/tcp/connect_task.hpp>
#include <tdk/event_loop/io/ip/tcp/read_task.hpp>
#include <tdk/event_loop/io/ip/tcp/close_task.hpp>
#include <tdk/event_loop/io/ip/tcp/write_task.hpp>
#include <vector>

namespace tdk{

class event_loop;

namespace io{
namespace ip{
namespace tcp {

class channel {
public:
	channel( tdk::event_loop& loop );
	~channel( void );

	void connect( const std::vector<tdk::io::ip::address>& addrs
			, tdk::io::ip::tcp::connect_task* ct );

	void connect_impl( tdk::io::ip::tcp::connect_task* ct );

	void read( void* p , int sz , tdk::io::ip::tcp::read_task* rt );
	void read( const tdk::io::buffer_adapter& buf , tdk::io::ip::tcp::read_task* rt );

	void read_impl( tdk::io::ip::tcp::read_task* rt );

	void write( void* p , int sz , tdk::io::ip::tcp::write_task* wt );
	void write( const tdk::io::buffer_adapter& buf , tdk::io::ip::tcp::write_task* wt );

	void write_impl( tdk::io::ip::tcp::write_task* wt );

	bool register_handle( void );

	void handle_io_event( void );
	void handle_readable( void );
	void handle_writeable( void );
	void handle_connect_event( void );

	tdk::io::ip::socket& socket();

	void handle_error( const std::error_code& ec , tdk::slist_queue<tdk::task>& queue );

	void close( tdk::io::ip::tcp::close_task* ct );
	void close_impl( tdk::io::ip::tcp::close_task* ct );

	tdk::event_loop& loop( void );
public:
	template < typename T_handler >
	void connect( const std::vector<tdk::io::ip::address>& addrs , const T_handler& handler );

	template < typename T_handler >
	void read( const tdk::io::buffer_adapter& buf , const T_handler& handler );

	template < typename T_handler >
	void write( const tdk::io::buffer_adapter& buf , const T_handler& handler );

	template < typename T_handler >
	void close( const T_handler& handler );
private:
	void _execute_task( tdk::task* t );
private:
	tdk::event_loop* _loop;
	tdk::io::epoll::task _channel_task;
	tdk::io::ip::socket _socket;
	tdk::slist_queue< tdk::task > _read_tasks;
	tdk::slist_queue< tdk::task > _write_tasks;
	tdk::slist_queue< tdk::task >* _complete_tasks;
public:
	static int write_try_count;
};

template < typename T_handler , typename T_base >
class handler_impl : public T_base {
public:
	handler_impl( const T_handler& handler )
		: T_base( &handler_impl::on_task , nullptr )
		, _handler( handler )
	{
	}

	~handler_impl( void ) {
	}

	static void on_task( tdk::task* t ) {
		handler_impl* impl = static_cast< handler_impl* >(t);
		(impl->_handler)(static_cast< T_base* >(impl));
		delete impl;
	}
private:
	T_handler _handler;
};

template < typename T_handler >
void channel::connect(
		const std::vector<tdk::io::ip::address>& addrs
		, const T_handler& handler )
{
	connect_task* task
		= new handler_impl< T_handler , connect_task >( handler );
	connect( addrs , task );
}

template < typename T_handler >
void channel::read(
		const tdk::io::buffer_adapter& buf
		, const T_handler& handler )
{
	read_task* task
			= new handler_impl< T_handler , read_task >( handler );
	read( buf , task );
}

template < typename T_handler >
void channel::write(
		const tdk::io::buffer_adapter& buf
		, const T_handler& handler )
{
	write_task* task
				= new handler_impl< T_handler , write_task >( handler );
	write( buf , task );
}

template < typename T_handler >
void channel::close( const T_handler& handler )
{
	class handler_impl2 : public close_task {
	public:
		handler_impl2( const T_handler& handler )
			: close_task( &handler_impl2::on_task , nullptr )
			, _handler( handler )
		{
		}

		~handler_impl2( void ) {
		}

		static void on_task( tdk::task* t ) {
			handler_impl2* impl = static_cast< handler_impl2* >(t);
			(impl->_handler)();
			delete impl;
		}
	private:
		T_handler _handler;
	};
	close_task* task = new handler_impl2( handler );
	close( task );
}

}}}}

#endif /* CHANNEL_HPP_ */
