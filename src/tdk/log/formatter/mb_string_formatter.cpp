#include "stdafx.h"
#include <tdk/log/formatter/mb_string_formatter.hpp>
#include <tdk/util/string.hpp>

#if defined( _WIN32 )

namespace tdk {
namespace log {

mb_string_formatter::mb_string_formatter( void ) {

}

mb_string_formatter::~mb_string_formatter( void ) {

}

void mb_string_formatter::format(  const record& r , tdk::buffer::memory_block& m ) {
	wchar_t unicode_buffer[8192];
	m.reserve( 8192 * sizeof( wchar_t ));
	tdk::time::tick::systemtime st = tdk::time::tick::to_systemtime( r.time.time());	
	int len = swprintf_s( unicode_buffer
		, 8192
		, L"[%04d%02d%02d %02d%02d%02d][%s][%s][%s][%s][%s:%d]\r\n"
		, st.wYear , st.wMonth , st.wDay , st.wHour , st.wMinute , st.wSecond
		, r.level_string()
		, r.category.name().c_str()
		, r.message
		, r.function_name
		, r.file_name
		, r.line_number
		);

	std::string msg = tdk::string::wcs_to_mbs( unicode_buffer );
	m.write( msg.c_str() );
}

formatter_ptr mb_string_formatter::instance( void ) {
	static formatter_ptr ptr( new mb_string_formatter() );
	return ptr;
}

}}

#endif
