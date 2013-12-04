#ifndef __tdk_log_logger_h__
#define __tdk_log_logger_h__

#include <tdk/tdk.hpp>
#include <tdk/log/record.hpp>
#include <tdk/log/writer/writer.hpp>
#include <tdk/log/formatter/formatter.hpp>
#include <tdk/threading/spin_lock.hpp>
#if defined( _WIN32)
#include <crtdefs.h>

#define LOG_D( tag , msg , ... ) do { tdk::log::logger(tag).write( tdk::log::level::log_debug , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)
#define LOG_I( tag , msg , ... ) do { tdk::log::logger(tag).write( tdk::log::level::log_info  , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)
#define LOG_W( tag , msg , ... ) do { tdk::log::logger(tag).write( tdk::log::level::log_warn  , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)
#define LOG_E( tag , msg , ... ) do { tdk::log::logger(tag).write( tdk::log::level::log_error , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)
#define LOG_F( tag , msg , ... ) do { tdk::log::logger(tag).write( tdk::log::level::log_fatal , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)

#define LOG_DUMP_D( tag , buffer , sz , msg , ... ) do { tdk::log::logger(tag).write_dump( tdk::log::level::log_debug ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOG_DUMP_I( tag , buffer , sz , msg , ... ) do { tdk::log::logger(tag).write_dump( tdk::log::level::log_info  ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOG_DUMP_W( tag , buffer , sz , msg , ... ) do { tdk::log::logger(tag).write_dump( tdk::log::level::log_warn  ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOG_DUMP_E( tag , buffer , sz , msg , ... ) do { tdk::log::logger(tag).write_dump( tdk::log::level::log_error ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOG_DUMP_F( tag , bzffer , sz , msg , ... ) do { tdk::log::logger(tag).write_dump( tdk::log::level::log_fatal ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)

#define LOGGER_D( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_debug , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)
#define LOGGER_I( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_info  , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)
#define LOGGER_W( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_warn  , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)
#define LOGGER_E( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_error , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)
#define LOGGER_F( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_fatal , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)

#define LOGGER_DUMP_D( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write_dump( tdk::log::level::log_debug ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_I( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write_dump( tdk::log::level::log_info  ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_W( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write_dump( tdk::log::level::log_warn  ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_E( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write_dump( tdk::log::level::log_error ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_F( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write_dump( tdk::log::level::log_fatal ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
/*
#define LOG_D( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_debug , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)
#define LOG_I( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_info  , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)
#define LOG_W( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_warn  , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)
#define LOG_E( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_error , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)
#define LOG_F( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_fatal , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , __VA_ARGS__ ); } while(0)

#define LOG_DUMP_D( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write_dump( tdk::log::level::log_debug ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOG_DUMP_I( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write_dump( tdk::log::level::log_info  ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOG_DUMP_W( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write_dump( tdk::log::level::log_warn  ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOG_DUMP_E( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write_dump( tdk::log::level::log_error ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOG_DUMP_F( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write_dump( tdk::log::level::log_fatal ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , __VA_ARGS__ ); } while(0)
*/
#else

#define LOG_D( tag , msg , ... ) do { tdk::log::logger(tag).write( tdk::log::level::log_debug , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , ##__VA_ARGS__ ); } while(0)
#define LOG_I( tag , msg , ... ) do { tdk::log::logger(tag).write( tdk::log::level::log_info  , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , ##__VA_ARGS__ ); } while(0)
#define LOG_W( tag , msg , ... ) do { tdk::log::logger(tag).write( tdk::log::level::log_warn  , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , ##__VA_ARGS__ ); } while(0)
#define LOG_E( tag , msg , ... ) do { tdk::log::logger(tag).write( tdk::log::level::log_error , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , ##__VA_ARGS__ ); } while(0)
#define LOG_F( tag , msg , ... ) do { tdk::log::logger(tag).write( tdk::log::level::log_fatal , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , ##__VA_ARGS__ ); } while(0)

#define LOG_DUMP_D( tag , buffer , sz , msg , ... ) do { tdk::log::logger(tag).write_dump( tdk::log::level::log_debug ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , ##__VA_ARGS__ ); } while(0)
#define LOG_DUMP_I( tag , buffer , sz , msg , ... ) do { tdk::log::logger(tag).write_dump( tdk::log::level::log_info  ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , ##__VA_ARGS__ ); } while(0)
#define LOG_DUMP_W( tag , buffer , sz , msg , ... ) do { tdk::log::logger(tag).write_dump( tdk::log::level::log_warn  ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , ##__VA_ARGS__ ); } while(0)
#define LOG_DUMP_E( tag , buffer , sz , msg , ... ) do { tdk::log::logger(tag).write_dump( tdk::log::level::log_error ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , ##__VA_ARGS__ ); } while(0)
#define LOG_DUMP_F( tag , bzffer , sz , msg , ... ) do { tdk::log::logger(tag).write_dump( tdk::log::level::log_fatal ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , ##__VA_ARGS__ ); } while(0)

#define LOGGER_D( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_debug , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , ##__VA_ARGS__ ); } while(0)
#define LOGGER_I( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_info  , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , ##__VA_ARGS__ ); } while(0)
#define LOGGER_W( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_warn  , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , ##__VA_ARGS__ ); } while(0)
#define LOGGER_E( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_error , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , ##__VA_ARGS__ ); } while(0)
#define LOGGER_F( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::level::log_fatal , _T(__FILE__) , __LINE__, _T(__FUNCTION__) , msg , ##__VA_ARGS__ ); } while(0)

#define LOGGER_DUMP_D( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write_dump( tdk::log::level::log_debug ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , ##__VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_I( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write_dump( tdk::log::level::log_info  ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , ##__VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_W( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write_dump( tdk::log::level::log_warn  ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , ##__VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_E( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write_dump( tdk::log::level::log_error ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , ##__VA_ARGS__ ); } while(0)
#define LOGGER_DUMP_F( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write_dump( tdk::log::level::log_fatal ,  _T(__FILE__) ,__LINE__ ,_T( __FUNCTION__ ), buffer , sz , msg , ##__VA_ARGS__ ); } while(0)

#endif
#include <vector>

namespace tdk{
namespace log{

class logger {
public:
	struct impl {
		tdk::log::level level;
		tdk::log::category category;
		tdk::threading::spin_lock lock;
		std::vector< writer_ptr > writers;
		impl( const tdk::log::category& cate );
		static logger::impl* get_instance( const tdk::tstring& cate ) ;
		static logger::impl* get_instance( const tdk::log::category& cate ) ;
	};
public:
	explicit logger( const tdk::tstring& cate );
#if defined( _WIN32 )
	explicit logger( const std::string& cate );
#endif
	explicit logger( const tdk::log::category& cate );
	explicit logger( logger::impl* impl );
    logger( const tdk::tstring& cate , writer_ptr ptr );
	~logger( void );

	

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

	void write_dump( 
		tdk::log::level level
		, const wchar_t* file 
		, const int line 
		, const wchar_t* function 
        , uint8_t* buffer 
        , int sz
		, const wchar_t* msg
		, ... );
#else
	void write( 
		tdk::log::level level
		, const char* file 
		, const int line 
		, const char* function 
		, const char* msg
		, ... );

	void write_dump( 
		tdk::log::level level
		, const char* file 
		, const int line 
		, const char* function 
        , uint8_t* buffer 
        , int sz
		, const char* msg
		, ... );
#endif
	void level( tdk::log::level l );

	void add_writer( const writer_ptr& writer );
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
