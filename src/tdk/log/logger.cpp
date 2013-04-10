#include "stdafx.h"
#include <tdk/log/logger.hpp>
#include <tdk/util/string.hpp>

namespace tdk{
namespace log{
namespace {

std::map< std::string , logger::impl* > _loggers;

}

struct logger::impl {
	tdk::log::level level;
	tdk::log::category category;
	
	std::vector< writer_ptr > writers;
	impl( const tdk::log::category& cate ) 
		: category( cate ) 
	{
		level = tdk::log::level::log_debug;
	}	

	static logger::impl* get_instance( const std::string& cate ) {
		return get_instance( tdk::log::category( cate ));
	}
	static logger::impl* get_instance( const tdk::log::category& cate ) {
		auto it = _loggers.find( cate.name() );
		if ( it == _loggers.end() ) {
			logger::impl* pimpl = new logger::impl(cate);
			_loggers.insert( std::make_pair( cate.name() , pimpl ));
			return pimpl;
		}
		return it->second;
	}
};


logger::logger( const std::string& cate ) 
	: _impl( impl::get_instance( cate ) ) 
{
}

logger::logger( const tdk::log::category& cate )
	: _impl( impl::get_instance( cate ) ) 
{
}

logger::~logger( void ) {

}

void logger::write( 
		tdk::log::level level
		, const char* file 
		, const int line 
		, const char* function 
		, const char* msg
		, ... )
{
	if ( level < _impl->level ) {
		return;
	}
	if ( msg != nullptr ) {
		char format_buffer[4096];
		va_list args;
		va_start( args , msg );
#if defined( _WIN32 ) 
		int len = _vsnprintf_s( format_buffer , 4096 , msg , args );
#else
		int len = vsnprintf( format_buffer , 4096 , msg , args );
#endif
		va_end( args );
		tdk::log::record log_record( level
			, _impl->category
#if defined( _WIN32 ) 
			, tdk::string::mbs_to_utf_8( format_buffer )
#else
			, format_buffer
#endif
			, file
			, line 
			, function );

#if defined( _WIN32 )
		if ( IsDebuggerPresent() ) {
			tdk::time::tick::systemtime st = tdk::time::tick::to_systemtime( log_record.time.time());
			char msg[4096];
			sprintf_s( msg 
				, "[%04d%02d%02d %02d%02d%02d][%s][%s][%s][%s][%s:%d]\r\n"
				, st.wYear , st.wMonth , st.wDay , st.wHour , st.wMinute , st.wSecond
				, log_record.level_string()
				, log_record.category.name().c_str()
				, format_buffer
				, log_record.function_name
				, log_record.file_name
				, log_record.line_number
				);
			OutputDebugStringA( msg );
		}	
#endif
		_write( log_record );
	}	
}

void logger::write( 
		tdk::log::level level
		, const char* file 
		, const int line 
		, const char* function 
		, const wchar_t* msg
		, ... )
{
	if ( level < _impl->level ) {
		return;
	}
	if ( msg != nullptr ) {
		wchar_t format_buffer[4096];
		va_list args;
		va_start( args , msg );
#if defined ( _WIN32 )
		int len = _vsnwprintf_s( format_buffer , 4096 , msg , args );
#else
		int len = vsnwprintf( format_buffer , 4096 , msg , args );
#endif
		va_end( args );

		tdk::log::record log_record( level
			, _impl->category
			, tdk::string::wcs_to_utf_8( format_buffer )
			, file
			, line 
			, function );

#if defined( _WIN32 )
		if ( IsDebuggerPresent() ) {
			tdk::time::tick::systemtime st = tdk::time::tick::to_systemtime( log_record.time.time());
			char prefix[4096];
			char postfix[4096];
			sprintf_s( prefix 
				, "[%04d%02d%02d %02d%02d%02d][%s][%s]["
				, st.wYear , st.wMonth , st.wDay , st.wHour , st.wMinute , st.wSecond
				, log_record.level_string()
				, log_record.category.name().c_str());
			sprintf_s( postfix 
				, "][%s][%s:%d]\r\n"
				, log_record.function_name
				, log_record.file_name
				, log_record.line_number
				);
			OutputDebugStringA( prefix );
			OutputDebugStringW( format_buffer );
			OutputDebugStringA( postfix );
		}	
#endif
		_write( log_record );
	}	
}

void logger::_write( const tdk::log::record& r ) {	
	for ( writer_ptr& ptr : _impl->writers ) {
		ptr->write( r );
	}
}

void logger::level( tdk::log::level l ) {
	_impl->level = l;
}

void logger::add_writer( writer_ptr& writer ) {
	_impl->writers.push_back( writer );
}

logger logger::get_instance( const std::string& cate ) {
	return tdk::log::logger( cate );
}

logger logger::get_instance( const tdk::log::category& cate ) {
	return tdk::log::logger( cate );
}

}}