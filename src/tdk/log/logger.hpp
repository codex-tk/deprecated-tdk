#ifndef __tdk_log_logger_h__
#define __tdk_log_logger_h__

#include <tdk/tdk.hpp>
#include <tdk/log/record.hpp>
#include <tdk/log/writer/writer.hpp>
#include <tdk/log/formatter/formatter.hpp>
#include <crtdefs.h>
#include <vector>

#define LOG_D( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_debug , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)
#define LOG_I( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_info  , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)
#define LOG_W( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_warn  , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)
#define LOG_E( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_error , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)
#define LOG_F( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_fatal , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)

#define LOG_DUMP_D( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write( tdk::log::level::log_debug ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOG_DUMP_I( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write( tdk::log::level::log_info  ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOG_DUMP_W( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write( tdk::log::level::log_warn  ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOG_DUMP_E( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write( tdk::log::level::log_error ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOG_DUMP_F( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write( tdk::log::level::log_fatal ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)

namespace tdk{
namespace log{

class logger {
public:
	struct impl {
		tdk::log::level level;
		tdk::log::category category;
	
		std::vector< writer_ptr > writers;
		impl( const tdk::log::category& cate );
		static logger::impl* get_instance( const tdk::tstring& cate ) ;
		static logger::impl* get_instance( const tdk::log::category& cate ) ;
	};
public:
	explicit logger( const tdk::tstring& cate );
	explicit logger( const tdk::log::category& cate );
	explicit logger( logger::impl* impl );
	~logger( void );

	void write( 
		tdk::log::level level
		, const char* file 
		, const int line 
		, const char* function 
		, const char* msg
		, ... );

#if defined(_WIN32) || defined(_WIN64)
	void write( 
		tdk::log::level level
		, const wchar_t* file 
		, const int line 
		, const wchar_t* function 
		, const wchar_t* msg
		, ... );

	void write( 
		tdk::log::level level
		, const wchar_t* file 
		, const int line 
		, const wchar_t* function 
		, const char* msg
		, ... );
#endif
	void level( tdk::log::level l );

	void add_writer( writer_ptr& writer );
private:
	void _write( const tdk::log::record& r );
public:	
	static logger get_instance( const tdk::tstring& cate );
	static logger get_instance( const tdk::log::category& cate );
	static logger::impl* get_impl_instance( const tdk::tstring& cate );
private:	
	impl* _impl;
};

}}

#endif