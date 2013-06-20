#include "stdafx.h"
#include <tdk/log/writer/console_writer.hpp>
#include <tdk/log/formatter/string_formatter.hpp>

namespace tdk {
namespace log {

console_writer::console_writer( const tdk::log::formatter_ptr& fmt )
	: writer( fmt ), _console( GetStdHandle( STD_OUTPUT_HANDLE ) ) 
{
	if ( _console == nullptr ) {
        AllocConsole();
		_console = GetStdHandle( STD_OUTPUT_HANDLE );
		/*
		HANDLE default_stdout = (HANDLE)0x00000007;
		if ( _console != default_stdout ) {
			SetStdHandle( STD_OUTPUT_HANDLE , default_stdout );
		}*/
	}
	//SetConsoleOutputCP( CP_UTF8 );	
}

console_writer::~console_writer( void ) {

}

void console_writer::write( const record& r ) {
	if ( _console == nullptr ) {
		return;
	}

	_formatter->format( r , _buffer );
	DWORD writes = 0;

	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo( _console , &info );
	static uint16_t colors[] = { 0x0a , 0x02 , 0x0b , 0x0c , 0x0e };
	SetConsoleTextAttribute( _console , colors[ static_cast<int>(r.level)] );
	DWORD dwWrite = 0;
	WriteFile( GetStdHandle(STD_OUTPUT_HANDLE)
		, _buffer.rd_ptr()
		, _buffer.length()
		, &dwWrite
		, nullptr);
	SetConsoleTextAttribute( _console , info.wAttributes );
	_buffer.clear();
}

writer_ptr console_writer::instance( void ) {
	static writer_ptr ptr( new console_writer( tdk::log::string_formatter::instance()));
	return ptr;
}

}}