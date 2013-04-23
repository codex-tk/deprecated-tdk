#include "stdafx.h"
#include <tdk/log/writer/file_writer.hpp>
#include <string>
/*
#include <caffe/config/env.hpp>
#include <caffe/time/date_time.hpp>
#include <caffe/threading/scoped_lock.hpp>
#include <caffe/diag/caffe_error.hpp>
#include <caffe/tools/string_op.hpp>
*/
namespace tdk {
namespace log {

file_writer::file_writer( const std::wstring name 
		, const tdk::log::formatter_ptr& fmt ) 
	: writer( fmt ) ,
	_file(INVALID_HANDLE_VALUE) ,
	_poxt_fix(name) ,
	_current_day(0) , 
	_line_count(0) , 
	_day_file_count(0){
}

file_writer::~file_writer( void ) {

}

void file_writer::write( const record& r ) {
	tdk::threading::scoped_lock<> guard(_lock);
	tdk::time::tick::systemtime st = tdk::time::tick::to_systemtime( tdk::date_time::local().time() );

	if( _file == INVALID_HANDLE_VALUE || _current_day  != st.wDay || _line_count > 65535 ) {
		if( _current_day != st.wDay ) {
			_delete_old_logs();
			_day_file_count = 0;
		}

		if( !_create_log_file() )
			return ;

		++_day_file_count;
		_line_count = 0;
        _current_day = st.wDay;
	}

    DWORD sz = 0;
    ++_line_count;

	_formatter->format( r , _buffer );
	::WriteFile( _file , _buffer.rd_ptr() , _buffer.length() , &sz , nullptr );
	_buffer.clear();
}

bool file_writer::_create_log_file( void ) {
	if ( _file != INVALID_HANDLE_VALUE ) 
		CloseHandle( _file );

	wchar_t filePath[MAX_PATH];
	GetModuleFileName( nullptr , filePath , MAX_PATH );
	wchar_t *fileName = wcsrchr( filePath , '\\' );
	if ( fileName ) {
		*(++fileName) = '\0';
	}else{
		memset( filePath , 0x00 , MAX_PATH * sizeof( wchar_t ));
	}
	
	std::wstring path = filePath;
    path.append( L"\\log\\" );

    LPSECURITY_ATTRIBUTES attr = nullptr;
    CreateDirectory( path.c_str() , attr );

	tdk::time::tick::systemtime st = tdk::time::tick::to_systemtime( tdk::date_time::local().time() );

    wchar_t file_name[4096];
    swprintf_s( file_name , 
                L"log_%04d%02d%02d_%02d%02d%02d_%04d_%s.log" ,
                st.wYear , st.wMonth , st.wDay , st.wHour , st.wMinute , st.wSecond ,
                _day_file_count , 
                _poxt_fix.c_str() );
    
    path.append( file_name );
	
    _file = CreateFile(  path.c_str() , 
                                GENERIC_READ | GENERIC_WRITE , 
                                FILE_SHARE_READ | FILE_SHARE_WRITE , 
                                NULL , 
                                CREATE_ALWAYS , 
                                FILE_ATTRIBUTE_NORMAL , 
                                NULL );

	if( _file == INVALID_HANDLE_VALUE )	
        return false;
	unsigned char bom[2];
	bom[0] = 0xff;
	bom[1] = 0xfe;
	DWORD sz = 0;
	::WriteFile( _file , bom , 2 , &sz , nullptr );
    return true;
}


void file_writer::_delete_old_logs( void ) {
	wchar_t filePath[MAX_PATH];
	GetModuleFileName( nullptr , filePath , MAX_PATH );
	wchar_t *fileName = wcsrchr( filePath , '\\' );
	if ( fileName ) {
		*(++fileName) = '\0';
	}else{
		memset( filePath , 0x00 , MAX_PATH * sizeof( wchar_t ));
	}
	
	std::wstring path = filePath;
    path.append( L"\\log\\" );

	tdk::date_time sep_date( tdk::date_time::local() - tdk::time_span::from_days( 30 ) );
	tdk::time::tick::systemtime st = tdk::time::tick::to_systemtime( tdk::date_time::local().time() );

    wchar_t file_name[4096];
	swprintf_s( file_name , L"log_%04d%02d%02d" , st.wYear , st.wMonth , st.wDay );

	std::wstring sep_file_name( file_name );

	std::wstring find_file_name;
    find_file_name.reserve( 4096 );

    WIN32_FIND_DATA find_data;
    HANDLE h = FindFirstFile( (path + L"*.*").c_str() , &find_data );
	if( h != INVALID_HANDLE_VALUE ){
		do {
            if ( !(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                find_file_name = find_data.cFileName;
				if( sep_file_name > find_file_name ) {
                    find_file_name = path + find_data.cFileName;
					DeleteFile( find_file_name.c_str());
				}
            }
		} while ( FindNextFile( h , &find_data ) == TRUE );
		FindClose( h );
	}
}

}}
