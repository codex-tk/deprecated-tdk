#ifndef __tdk_task_h__
#define __tdk_task_h__

#include <tdk/util/list_node.hpp>

namespace tdk {

class task 
	: public tdk::slist_node< task >{
public:
	typedef void (*handler)( task* );

	task( void );
	task( handler h , void* ctx = nullptr );
	~task( void );

	void set_handler( handler h , void* ctx = nullptr );

	void* context( void );

	void operator()( void );
private:
	handler _handler;
	void*	_ctx;
};

}

#endif