#ifndef __tdk_log_console_writer_h__
#define __tdk_log_console_writer_h__

#include <tdk/log/writer/writer.hpp>
#include <tdk/log/record.hpp>
#if defined( _WIN32 )
namespace tdk {
namespace log {

class console_writer : public writer {
public:
	console_writer( const tdk::log::formatter_ptr& fmt );
	virtual ~console_writer( void );
	virtual void write( const record& r );

	static writer_ptr instance( void );
private:
	tdk::buffer::memory_block _buffer;
	HANDLE _console;
};

}}
#endif
#endif
