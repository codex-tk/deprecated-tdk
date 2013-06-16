#ifndef __tdk_error_category_tdk_h__
#define __tdk_error_category_tdk_h__

#include <system_error>
#include <tdk/error/error_category.hpp>

namespace tdk {

enum class errc {
	tdk_success = 0 ,
	tdk_tls_not_initialized ,
	tdk_user_abort ,
	tdk_network_user_abort = 0x1000 ,
	tdk_network_remote_closed ,
};
class error_category_tdk : public tdk::error_category {
public:
	error_category_tdk (void);
	virtual ~error_category_tdk (void) ;
	virtual const TCHAR *name() const;
	virtual tdk::tstring message(int _Errval) const ;
};

}

#endif