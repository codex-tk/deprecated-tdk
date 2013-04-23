#include "stdafx.h"
#include <tdk/log/logger.hpp>
#include <tdk/util/string.hpp>

namespace tdk{
namespace log{
namespace {

std::map< tdk::tstring  , logger::impl* > _loggers;

}

logger::impl::impl( const tdk::log::category& cate ) 
			: category( cate ) 
		{
			level = tdk::log::level::log_debug;
		}	

logger::impl* logger::impl::get_instance( const tdk::tstring& cate ) {
	return get_instance( tdk::log::category( cate ));
}

logger::impl* logger::impl::get_instance( const tdk::log::category& cate ) {
	auto it = _loggers.find( cate.name() );
	if ( it == _loggers.end() ) {
		logger::impl* pimpl = new logger::impl(cate);
		_loggers.insert( std::make_pair( cate.name() , pimpl ));
		return pimpl;
	}
	return it->second;
}


logger::logger( const tdk::tstring& cate ) 
	: _impl( impl::get_instance( cate ) ) 
{
}

logger::logger( const tdk::log::category& cate )
	: _impl( impl::get_instance( cate ) ) 
{
}

logger::logger( logger::impl* impl )
	: _impl( impl )
{
}

logger::~logger( void ) {

}

#if defined(_WIN32) || defined(_WIN64)

void logger::write( 
		tdk::log::level level
		, const wchar_t* file 
		, const int line 
		, const wchar_t* function 
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
		_vsnprintf_s( format_buffer , 4096 , msg , args );
		va_end( args );
		tdk::log::record log_record( level
			, _impl->category
			, tdk::string::mbs_to_wcs(format_buffer)
			, file
			, line 
			, function );

		if ( IsDebuggerPresent() ) {
			tdk::time::tick::systemtime st = tdk::time::tick::to_systemtime( log_record.time.time());
			wchar_t msg[4096];
			swprintf_s( msg 
				, L"[%04d%02d%02d %02d%02d%02d][%s][%s][%s][%s][%s:%d]\r\n"
				, st.wYear , st.wMonth , st.wDay , st.wHour , st.wMinute , st.wSecond
				, log_record.level_string()
				, log_record.category.name().c_str()
				, log_record.message.c_str()
				, log_record.function_name
				, log_record.file_name
				, log_record.line_number
				);
			OutputDebugString( msg );
		}	
		_write( log_record );
	}	
}

#if defined(_WIN32) || defined(_WIN64)
void logger::write( 
		tdk::log::level level
		, const wchar_t* file 
		, const int line 
		, const wchar_t* function 
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
		_vsnwprintf_s( format_buffer , 4096 , msg , args );
		va_end( args );

		tdk::log::record log_record( level
			, _impl->category
			, format_buffer
			, file
			, line 
			, function );

		if ( IsDebuggerPresent() ) {
			tdk::time::tick::systemtime st = tdk::time::tick::to_systemtime( log_record.time.time());
			wchar_t msg[4096];
			swprintf_s( msg 
				, L"[%04d%02d%02d %02d%02d%02d][%s][%s][%s][%s][%s:%d]\r\n"
				, st.wYear , st.wMonth , st.wDay , st.wHour , st.wMinute , st.wSecond
				, log_record.level_string()
				, log_record.category.name().c_str()
				, format_buffer
				, log_record.function_name
				, log_record.file_name
				, log_record.line_number
				);
			OutputDebugString( msg );
		}	
		_write( log_record );
	}	
}
#endif


#else
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
		int len = vsnprintf( format_buffer , 4096 , msg , args );
		va_end( args );
		tdk::log::record log_record( level
			, _impl->category
			, format_buffer
			, file
			, line 
			, function );
		_write( log_record );
	}	
}
#endif

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

logger logger::get_instance( const tdk::tstring& cate ) {
	return tdk::log::logger( cate );
}

logger logger::get_instance( const tdk::log::category& cate ) {
	return tdk::log::logger( cate );
}

logger::impl* logger::get_impl_instance( const tdk::tstring& cate ) {
	return impl::get_instance( cate );
}

}}