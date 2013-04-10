#ifndef __tdk_task_results_h__
#define __tdk_task_results_h__

namespace tdk{
namespace task {

enum class IO_EVENT {
	ACCEPT 
	, CONNECT
	, RECV
	, SEND
};

struct IO_CONTEXT : OVERLAPPED {
	IO_EVENT event;
};

struct ACCEPT_IO_CONTEXT : public IO_CONTEXT {
	ACCEPT_IO_CONTEXT( void );
};

struct CONNECT_IO_CONTEXT : public IO_CONTEXT {
	CONNECT_IO_CONTEXT( void );
};

struct RECV_IO_CONTEXT : public IO_CONTEXT {
	RECV_IO_CONTEXT( void );
};

struct SEND_IO_CONTEXT : public IO_CONTEXT {
	SEND_IO_CONTEXT( void );
};

}}

#endif