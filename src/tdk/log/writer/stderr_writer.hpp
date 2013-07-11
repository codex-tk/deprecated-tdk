#ifndef __tdk_log_stderr_writer_h__
#define __tdk_log_stderr_writer_h__

#include <tdk/log/writer/writer.hpp>
#include <tdk/log/record.hpp>

namespace tdk {
namespace log {

class stderr_writer : public writer {
public:
	stderr_writer( const tdk::log::formatter_ptr& fmt );
	virtual ~stderr_writer( void );
	virtual void write( const record& r );

	static writer_ptr instance( void );
private:
	tdk::buffer::memory_block _buffer;
};

}}

#endif
