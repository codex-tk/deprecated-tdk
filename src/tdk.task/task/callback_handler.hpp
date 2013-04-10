#ifndef __tdk_task_callback_handler_h__
#define __tdk_task_callback_handler_h__

namespace tdk {
namespace task {

template < typename T_handler , typename T_ctx >
class callback_handler {
public:

private:
	T_handler _handler;
	T_ctx	  _ctx;
};

}}
#endif