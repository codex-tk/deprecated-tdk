#ifndef __tdk_io_engine_scheduler_h__
#define __tdk_io_engine_scheduler_h__

#include <tdk/io/engine_epoll.hpp>
#include <tdk/task/timer_handler.hpp>
#include <tdk/util/list_node.hpp>
#include <tdk/io/engine_epoll.hpp>
#include <list>

namespace tdk {
namespace io {
/*
class engine::scheduler {
public:
	scheduler( tdk::io::engine& en );
	~scheduler( void );


    void post( tdk::io::operation* op );
    
    void schedule( tdk::io::engine::timer_id& id );
	bool cancel( tdk::io::engine::timer_id& id );

    void drain_expired( void );
    void drain_cancels( void );
    void drain_op_queue( void );
    void handle_event( int evt );

    tdk::io::operation* fetch( void );

    void set_wakeup_time( void );
private:
    tdk::io::timerfd _timerfd;
    tdk::io::engine::context _context;
    tdk::threading::spin_lock _lock;
    tdk::slist_queue< tdk::io::operation > _op_queue;
	tdk::io::engine& _engine;
	std::list< tdk::io::engine::timer_id > _timers;
	std::list< tdk::io::engine::timer_id > _cancels;
/*	
	void open( void );
	void close( void );

	void schedule( tdk::io::engine::timer_id& id );
	bool cancel( tdk::io::engine::timer_id& id );

	void drain( void );

	void drain_op_queue( void );
	void drain_cancels( void );
	void drain_expired( void );

	static void __stdcall _on_complete( operation* op );
private:
	tdk::io::engine& _engine;
	mutable tdk::threading::spin_lock _lock;
	std::list< tdk::io::engine::timer_id > _timers;
	std::list< tdk::io::engine::timer_id > _cancels;
    tdk::slist_queue _op_queue;
    */
};

}}


#endif
