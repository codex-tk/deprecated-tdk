#include "stdafx.h"
#include <tdk/log/writer/printf_writer.hpp>
#include <tdk/log/formatter/string_formatter.hpp>

namespace tdk {
namespace log {

printf_writer::printf_writer( const tdk::log::formatter_ptr& fmt )
	: writer( fmt )
{
}

printf_writer::~printf_writer( void ) {

}

void printf_writer::write( const record& r ) {
	_formatter->format( r , _buffer );
    fwrite( _buffer.rd_ptr() , 1 ,  _buffer.length() , stderr  );
    _buffer.clear();
}

writer_ptr printf_writer::instance( void ) {
	static writer_ptr ptr( new printf_writer( tdk::log::string_formatter::instance()));
	return ptr;
}

}}
