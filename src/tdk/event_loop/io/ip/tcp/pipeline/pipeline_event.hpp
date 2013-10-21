#ifndef __tdk_pipeline_event_h__
#define __tdk_pipeline_event_h__

#include <tdk/buffer/memory_block.hpp>

namespace tdk {
namespace io {
namespace ip {
namespace tcp {

class message {
public:
	message( void );
	message( int type , int sz );
	message( int type , const tdk::buffer::memory_block& mb );
	message( const message& m );
	message& operator=( const message& m );

	~message( void );

	int type( void );
	void type( int t );

	tdk::buffer::memory_block& data( void );
	void data( const tdk::buffer::memory_block& mb );
private:
	int _type;
	tdk::buffer::memory_block _mb;
};

/*
class connect_event : public tcp::pipeline::event {
};

class close_event : public tcp::pipeline::event {
};

class error_event : public tcp::pipeline::event {
public:
	error_event( void ){}

	void error( const std::error_code& ec );
	std::error_code error( void );
private:
	std::error_code _ec;
};


class recv_event : public tcp::pipeline::event {
public:

	std::vector< message >& messages( void );
	void clear( void );
	void add_message( const message& m );
private:
	std::vector< message > _messages;
};*/

}}}}

#endif
