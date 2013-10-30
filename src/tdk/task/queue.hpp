#ifndef __tdk_task_queue_h__
#define __tdk_task_queue_h__

#include <tdk/util/list_node.hpp>
#include <tdk/threading/spin_lock.hpp>

namespace tdk{
namespace task {

class queue {
public:
	class operation 
		: public tdk::slist_node< operation >{
	public:
		typedef void (*handler)( operation* op );
		operation( queue* q );
		~operation( void );

		void set_handler( handler h );
		void  context( void* ctx );
		void* context( void );

		void dispatch( void );

		void operator()( void );
	private:
		queue* _queue;
		handler _handler;
		void* _context;
	};

	queue( void );
	~queue( void );

	void execute( operation* op );
	bool fetch( void );
private:
	tdk::threading::spin_lock _lock;
	tdk::slist_queue< operation > _op_queue;
	operation* _running;
};

}}

#endif