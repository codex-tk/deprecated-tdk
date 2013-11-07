#ifndef __tdk_log_string_formatter_h__
#define __tdk_log_string_formatter_h__

#include <tdk/log/formatter/formatter.hpp>
#include <tdk/log/record.hpp>
#if defined( _WIN32 )
namespace tdk {
namespace log {

class mb_string_formatter : public formatter {
public:
	mb_string_formatter ( void );
	virtual ~mb_string_formatter ( void );
	virtual void format(  const record& r , tdk::buffer::memory_block& m );

	static formatter_ptr instance( void );
};

}}
#endif
#endif
