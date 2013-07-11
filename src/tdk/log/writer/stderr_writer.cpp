#include "stdafx.h"
#include <tdk/log/writer/stderr_writer.hpp>
#include <tdk/log/formatter/string_formatter.hpp>

namespace tdk {
namespace log {

stderr_writer::stderr_writer( const tdk::log::formatter_ptr& fmt )
	: writer( fmt )
{
}

stderr_writer::~stderr_writer( void ) {

}

void stderr_writer::write( const record& r ) {
	_formatter->format( r , _buffer );
    fwrite( _buffer.rd_ptr() , 1 ,  _buffer.length() , stderr  );
    _buffer.clear();
}

writer_ptr stderr_writer::instance( void ) {
	static writer_ptr ptr( new stderr_writer( tdk::log::string_formatter::instance()));
	return ptr;
}

}}
