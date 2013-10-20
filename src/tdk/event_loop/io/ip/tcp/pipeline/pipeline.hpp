#ifndef __tdk_tcp_pipeline_h__
#define __tdk_tcp_pipeline_h__

namespace tdk {

class event_loop;

namespace io {
namespace ip {
namespace tcp {

class filter;
class pipeline {
public:
	class event {
	public:
		event( void );
		~event( void );

		bool propagation( void );
		void propagation( bool b );
	private:
		bool _propagation;
	};
public:
	pipeline( tdk::event_loop& loop );
	~pipeline( void );

	tdk::event_loop& loop( void );

	void add_last( filter* f );

public:
	void fire_on_connect( void );
	void fire_on_recv( void );
	void fire_on_error( const std::error_code& ec );
	void fire_on_close( void );
private:
	tdk::event_loop& _loop;
	filter* _begin;
	filter* _end;
};

}}}}

#endif
