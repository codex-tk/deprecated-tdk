#ifndef __tdk_io_engine_epoll_h__
#define __tdk_io_engine_epoll_h__

#include <tdk/tdk.hpp>
#include <tdk/time/date_time.hpp>
#include <tdk/io/eventfd.hpp>
#include <tdk/io/engine_detail.hpp>
#include <tdk/util/rc_ptr.hpp>
#include <tdk/io/timerfd.hpp>
#include <list>
namespace tdk {
namespace io{

class engine {
public:
    struct context{
        void* object;
        void (*callback)(void* obj , int event);
        void operator()( int evt ){
            if ( callback != nullptr  ) 
                (*callback)( object , evt );
        }
    };
public:
    engine( void );
    ~engine( void );

    void wake_up( void );

    bool ctl( int op ,  int fd , int ev , void* ptr );

    void post( tdk::io::operation* op , const std::error_code& ec );   
//    void drain(void);

    void run( void );
private:
//    tdk::io::operation* fetch( void );
public:
    template < typename T_handler >
    void post( const T_handler& h ) {
        class op_base : public tdk::io::operation {
        public:
            op_base( tdk::io::operation::callback cb )
                : operation( cb ){}
            bool end_operation( void ){ return true; }
        };
        typedef tdk::io::detail::operation_impl<
            T_handler , op_base , tdk::io::detail::dispatcher_none_param 
            > handler_op;
        handler_op* op = new handler_op( h );
        post( op , std::error_code());
    }
    int wait( const tdk::time_span& ts );

    void inc_posted( void );
    void dec_posted( void );
public:

	class timer_operation : public operation 
		, public tdk::rc_ptr_base< timer_operation >
	{
	public:
		timer_operation( operation::callback cb 
			, const tdk::date_time& expired );
		virtual ~timer_operation( void );

		bool end_operation();
						
		void expired_at( const tdk::date_time& at );
		const tdk::date_time& expired_at(void);
	private:
		tdk::date_time _expired_at;
	};
public:
    typedef tdk::rc_ptr< timer_operation > timer_id;
    class scheduler {
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
    };
	
	template < typename T_handler >
	timer_id schedule( const tdk::date_time& dt 
		, const T_handler& h )
	{
		typedef tdk::io::detail::operation_impl< T_handler 
		, tdk::io::engine::timer_operation
		, detail::dispatcher_error_code
		, detail::release_policy_release
		> impl;
		timer_id id( new impl( h , dt ));
		return schedule(id);
	}
	timer_id schedule( timer_id& id );
	void cancel( timer_id& id );	
private:
    int _epoll_fd;
    std::atomic<int> _posted;
    scheduler* _scheduler;
};


}}


#endif
