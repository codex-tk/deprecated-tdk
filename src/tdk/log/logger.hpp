#ifndef __tdk_log_logger_h__
#define __tdk_log_logger_h__

#include <tdk/tdk.hpp>
#include <tdk/log/record.hpp>
#include <tdk/log/writer/writer.hpp>
#include <tdk/log/formatter/formatter.hpp>
#include <crtdefs.h>
#include <vector>

#define LOG_D( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::LEVEL::LOG_DEBUG ,  __FILE__ , __LINE__ , __FUNCTION__ , msg , __VA_ARGS__ ); } while(0)
#define LOG_I( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::LEVEL::LOG_INFO  ,  __FILE__ , __LINE__ , __FUNCTION__ , msg , __VA_ARGS__ ); } while(0)
#define LOG_W( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::LEVEL::LOG_WARN  ,  __FILE__ , __LINE__ , __FUNCTION__ , msg , __VA_ARGS__ ); } while(0)
#define LOG_E( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::LEVEL::LOG_ERROR ,  __FILE__ , __LINE__ , __FUNCTION__ , msg , __VA_ARGS__ ); } while(0)
#define LOG_F( logger_instance , msg , ... ) do { logger_instance.write( tdk::log::LEVEL::LOG_FATAL ,  __FILE__ , __LINE__ , __FUNCTION__ , msg , __VA_ARGS__ ); } while(0)

#define LOG_DUMP_D( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write( tdk::log::LEVEL::LOG_DEBUG ,  __FILE__ , __LINE__ , __FUNCTION__ , buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOG_DUMP_I( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write( tdk::log::LEVEL::LOG_INFO  ,  __FILE__ , __LINE__ , __FUNCTION__ , buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOG_DUMP_W( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write( tdk::log::LEVEL::LOG_WARN  ,  __FILE__ , __LINE__ , __FUNCTION__ , buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOG_DUMP_E( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write( tdk::log::LEVEL::LOG_ERROR ,  __FILE__ , __LINE__ , __FUNCTION__ , buffer , sz , msg , __VA_ARGS__ ); } while(0)
#define LOG_DUMP_F( logger_instance , buffer , sz , msg , ... ) do { logger_instance.write( tdk::log::LEVEL::LOG_FATAL ,  __FILE__ , __LINE__ , __FUNCTION__ , buffer , sz , msg , __VA_ARGS__ ); } while(0)

namespace tdk{
namespace log{

class logger {
public:
	struct impl;
public:
	explicit logger( const std::string& cate );
	explicit logger( const tdk::log::category& cate );

	~logger( void );

	void write( 
		tdk::log::LEVEL level
		, const char* file 
		, const int line 
		, const char* function 
		, const char* msg
		, ... );

	void write( 
		tdk::log::LEVEL level
		, const char* file 
		, const int line 
		, const char* function 
		, const wchar_t* msg
		, ... );

	void level( tdk::log::LEVEL l );

	void add_writer( writer_ptr& writer );
private:
	void _write( const tdk::log::record& r );
public:	
	static logger get_instance( const std::string& cate );
	static logger get_instance( const tdk::log::category& cate );
private:	
	impl* _impl;
};

}}

#endif