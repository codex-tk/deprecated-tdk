#ifndef __tdk_tcp_pipeline_h__
#define __tdk_tcp_pipeline_h__

#include <tdk/io/ip/socket.hpp>
#include <tdk/event_loop/io/task.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/pipeline_event.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/filter_chain.hpp>
#include <tdk/event_loop/io/ip/tcp/pipeline/config.hpp>
#include <tdk/util/rc_ptr.hpp>
#include <atomic>
#include <list>

namespace tdk {
class event_loop;
namespace io {
namespace ip {
namespace tcp {
class filter;
class pipeline : public tdk::rc_ptr_base< pipeline > {
public:
	pipeline( tdk::event_loop& loop
			, const tdk::io::ip::tcp::config& cfg
			, int fd );
	~pipeline( void );

	tdk::event_loop& loop( void );

	void add( filter* f );

	void close( void );
	void write( tcp::message& msg );

	void error_propagation( const std::error_code& err );

public:
	void handle_readable( void );
	void handle_writeable( void );

	void do_write( tcp::message& msg );
private:
	void _error_propagation(const std::error_code& err );
	void _error_propagation_internal(const std::error_code& err );
	void _on_closed( void );
	void _do_write( tcp::message& msg );
	bool _send_remains( void );
public:
	void on_connected( void );
	void on_accepted( const tdk::io::ip::address& addr );
public:
	static void _handle_io_events( tdk::task* t );
	void handle_io_events( void );
private:
	tdk::event_loop& _loop;
	const tdk::io::ip::tcp::config& _config;
	tdk::io::ip::socket _socket;
	filter_chain _chain;
	tdk::io::task _io_context;
	std::atomic<int> _state;
	std::list< tcp::message > _send_queue;
};

}}}}

#endif
