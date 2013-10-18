#ifndef __tdk_tcp_pipeline_h__
#define __tdk_tcp_pipeline_h__

namespace tdk {

class event_loop;

namespace io {
namespace ip {
namespace tcp {

class pipeline {
public:
	pipeline( tdk::event_loop& loop );
	~pipeline( void );

	 tdk::event_loop& loop( void );
private:
	tdk::event_loop& _loop;
};

}}}}

#endif
