#ifndef __tdk_log_printf_writer_h__
#define __tdk_log_printf_writer_h__

#include <tdk/log/writer/writer.hpp>
#include <tdk/log/record.hpp>

namespace tdk {
namespace log {

class printf_writer : public writer {
public:
	printf_writer( const tdk::log::formatter_ptr& fmt );
	virtual ~printf_writer( void );
	virtual void write( const record& r );

	static writer_ptr instance( void );
private:
	tdk::buffer::memory_block _buffer;
};

}}

#endif
