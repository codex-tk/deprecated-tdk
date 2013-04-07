#ifndef __tdk_log_formatter_h__
#define __tdk_log_formatter_h__

#include <tdk/buffer/memory_block.hpp>
#include <memory>

namespace tdk {
namespace log {

struct record;
class formatter{
public:
	formatter( void );
	virtual ~formatter( void );
	virtual void format( const record& r , tdk::buffer::memory_block& m ) = 0;
};

typedef std::tr1::shared_ptr< formatter > formatter_ptr;

}}

#endif