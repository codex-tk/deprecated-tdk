#ifndef __tdk_diag_mini_dump_handler_h__
#define __tdk_diag_mini_dump_handler_h__

#include <tdk/util/win32/global_seh.hpp>
#include <tdk/util/win32/mini_dump.hpp>

namespace tdk {
namespace util {

class mini_dump_seh_handler: public seh_handler {
public:
	mini_dump_seh_handler( MINIDUMP_TYPE type );
	virtual ~mini_dump_seh_handler(void);
	virtual void handle_exception( EXCEPTION_POINTERS *pExp );
private:
	void _write_dump( EXCEPTION_POINTERS* pExp );
private:
	mini_dump _dump;
};

}}

#endif