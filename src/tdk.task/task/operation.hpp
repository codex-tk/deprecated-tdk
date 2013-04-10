#ifndef __tdk_task_operation_h__
#define __tdk_task_operation_h__

namespace tdk {
namespace task {

class operation {
public:
	operation( void );
	virtual ~operation( void );
	virtual void operator()(void ) = 0;
};

}}

#endif