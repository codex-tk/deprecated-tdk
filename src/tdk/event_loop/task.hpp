#ifndef __tdk_task_h__
#define __tdk_task_h__

#include <tdk/util/list_node.hpp>
#include <functional>

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
public:
	template < typename T_handler >
	static tdk::task* make_one_shot_task( const T_handler& h ) {
		class handler_task_auto_delete : public tdk::task {
		public:
			handler_task_auto_delete( const T_handler& h )
				: tdk::task( handler_task_auto_delete::on_task , nullptr )
				, _handler(h)
			{
			}

			~handler_task_auto_delete( void ) {
			}

			static void on_task( tdk::task* t ) {
				handler_task_auto_delete* impl =
						static_cast<handler_task_auto_delete*>(t);
				impl->_handler();
				delete impl;
			}
		private:
			T_handler _handler;
		};
		return new handler_task_auto_delete(h);
	}
};


template < typename T_base , typename R = void >
class handler_task : public T_base {
public:
	typedef std::function< R (T_base*) > handler_type;
	handler_task( const handler_type& h )
		: T_base( &handler_task::on_task , nullptr )
		, _handler( h )
	{
	}

	~handler_task( void ) {

	}

	static void on_task( tdk::task* t ) {
		handler_task< T_base , R >* impl
			= static_cast< handler_task< T_base , R >* >(t);
		(impl->_handler)(impl);
	}
private:
	handler_type _handler;
};


}

#endif
