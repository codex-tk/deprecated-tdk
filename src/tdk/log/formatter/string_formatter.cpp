#include "stdafx.h"
#include <tdk/log/formatter/string_formatter.hpp>

namespace tdk {
namespace log {

string_formatter::string_formatter( void ) {

}

string_formatter::~string_formatter( void ) {

}
#if defined ( _WIN32 ) 
void string_formatter::format(  const record& r , tdk::buffer::memory_block& m ) {
	m.reserve( 8192 * sizeof( wchar_t ));
	tdk::time::tick::systemtime st = tdk::time::tick::to_systemtime( r.time.time());	
	int len = swprintf_s( reinterpret_cast< wchar_t* >(m.wr_ptr())
		, m.space() / sizeof( wchar_t )
		, L"[%04d%02d%02d %02d%02d%02d][%s][%s][%s][%s][%s:%d]\r\n"
		, st.wYear , st.wMonth , st.wDay , st.wHour , st.wMinute , st.wSecond
		, r.level_string()
		, r.category.name().c_str()
		, r.message
		, r.function_name
		, r.file_name
		, r.line_number
		);
	m.wr_ptr( len * sizeof( wchar_t ) );
}

#elif defined( linux ) || defined ( __linux )
void string_formatter::format(  const record& r , tdk::buffer::memory_block& m ) {
	m.reserve( 8192 );
	tdk::time::tick::systemtime st = tdk::time::tick::to_systemtime( r.time.time());	
	int len = snprintf(reinterpret_cast<char*>( m.wr_ptr())
		, m.space()	
        , "[%04d%02d%02d %02d%02d%02d][%s][%s][%s][%s][%s:%d]\r\n"
		, st.wYear , st.wMonth , st.wDay , st.wHour , st.wMinute , st.wSecond
		, r.level_string()
		, r.category.name().c_str()
		, r.message
		, r.function_name
		, r.file_name
		, r.line_number
		);
	m.wr_ptr( len );
}

#elif defined ( __MACOSX__ ) || defined ( __APPLE__ ) 

#else

#endif




formatter_ptr string_formatter::instance( void ) {
	static formatter_ptr ptr( new string_formatter() );
	return ptr;
}

}}
