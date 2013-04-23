#include "stdafx.h"
#include <tdk/log/formatter/string_formatter.hpp>

namespace tdk {
namespace log {

string_formatter::string_formatter( void ) {

}

string_formatter::~string_formatter( void ) {

}

void string_formatter::format(  const record& r , tdk::buffer::memory_block& m ) {
	m.reserve( 4096 );
	tdk::time::tick::systemtime st = tdk::time::tick::to_systemtime( r.time.time());	
	int len = swprintf_s( reinterpret_cast< wchar_t* >(m.wr_ptr())
		, m.space() / sizeof( wchar_t )
		, L"[%04d%02d%02d %02d%02d%02d][%s][%s][%s][%s][%s:%d]\r\n"
		, st.wYear , st.wMonth , st.wDay , st.wHour , st.wMinute , st.wSecond
		, r.level_string()
		, r.category.name().c_str()
		, r.message.c_str()
		, r.function_name
		, r.file_name
		, r.line_number
		);
	m.wr_ptr( len * sizeof( wchar_t ) );
}

formatter_ptr string_formatter::instance( void ) {
	static formatter_ptr ptr( new string_formatter() );
	return ptr;
}

}}