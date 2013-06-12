#ifndef __tdk_diag_mini_dump_handler_h__
#define __tdk_diag_mini_dump_handler_h__

#include <tdk/diag/global_seh_win32.hpp>
#include <tdk/diag/mini_dump_win32.hpp>

namespace tdk {
namespace diag {

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