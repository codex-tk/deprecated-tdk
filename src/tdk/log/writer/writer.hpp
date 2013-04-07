#ifndef __tdk_log_writer_h__
#define __tdk_log_writer_h__

#include <tdk/log/formatter/formatter.hpp>
#include <memory>

namespace tdk{
namespace log{

struct record;
class writer {
public:
	writer( const formatter_ptr& fmt );
	virtual ~writer( void );

	virtual void write( const tdk::log::record& r ) = 0;
protected:
	formatter_ptr _formatter;
};

typedef std::tr1::shared_ptr<writer> writer_ptr;

}}

#endif