#ifndef __tdk_task_event_loop_object_h__
#define __tdk_task_event_loop_object_h__

namespace tdk {
namespace task {

class event_loop;
class event_loop_object {
public:
	event_loop_object( event_loop& loop );

	void loop( event_loop* l );
	event_loop& loop( void );
protected:
	~event_loop_object( void );
private:
	event_loop* _loop;
};


}}

#endif