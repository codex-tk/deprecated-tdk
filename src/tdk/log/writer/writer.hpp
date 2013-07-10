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
#if defined ( _WIN32 ) 
typedef std::tr1::shared_ptr<writer> writer_ptr;
#elif defined( linux ) || defined ( __linux )
typedef std::shared_ptr<writer> writer_ptr;
#elif defined ( __MACOSX__ ) || defined ( __APPLE__ ) 

#else

#endif



}}

#endif
