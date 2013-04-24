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
	if ( level < _impl->level ) 
		return;
	if ( msg != nullptr ) {
		tdk::log::record log_record( level , _impl->category , file , line  , function );
		do {
			char fmt_buffer[k_log_buffer_size];
			va_list args;
			va_start( args , msg );
			int len = _vsnprintf_s( fmt_buffer , k_log_buffer_size , msg , args );
			va_end( args );
			MultiByteToWideChar( CP_ACP , 0 , fmt_buffer , -1 , log_record.message , len );
		} while (0);
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
	if ( level < _impl->level ) 
		return;
	if ( msg != nullptr ) {
		tdk::log::record log_record( level , _impl->category , file , line  , function );
		va_list args;
		va_start( args , msg );
		_vsnwprintf_s( log_record.message , k_log_buffer_size , _TRUNCATE , msg , args );
		va_end( args );
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
		tdk::log::record log_record( level , _impl->category , file , line  , function );
		va_list args;
		va_start( args , msg );
		int len = vsnprintf( log_record.message , k_log_buffer_size , msg , args );
		va_end( args );
		_write( log_record );
	}	
}
#endif

void logger::_write( const tdk::log::record& r ) {	
#if defined( _WIN32 )
	if ( IsDebuggerPresent() ) {
		tdk::time::tick::systemtime st = tdk::time::tick::to_systemtime( r.time.time());
		wchar_t msg[k_log_buffer_size];
		swprintf_s( msg , L"[%04d%02d%02d %02d%02d%02d][%s][%s]["
			, st.wYear , st.wMonth , st.wDay , st.wHour , st.wMinute , st.wSecond
			, r.level_string()
			, r.category.name().c_str());
		OutputDebugString( msg );
		OutputDebugString( r.message );
		swprintf_s( msg , L"][%s][%s:%d]\r\n"
			, r.function_name
			, r.file_name
			, r.line_number );
		OutputDebugString( msg );
	}	
#endif
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