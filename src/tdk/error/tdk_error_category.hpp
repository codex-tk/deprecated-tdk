#ifndef __tdk_tdk_error_castgory_h__
#define __tdk_tdk_error_castgory_h__

#include <system_error>
#include <tdk/error/error_category.hpp>

namespace tdk {

enum class errc {
	tdk_success = 0 ,
	tdk_tls_not_initialized ,

	tdk_network_user_abort = 0x1000 ,
	tdk_network_remote_closed ,
};
class tdk_error_category : public tdk::error_category {
public:
	tdk_error_category (void);
	virtual ~tdk_error_category (void) ;
	virtual const char *name() const;
	virtual std::string message(int _Errval) const ;
};

}

#endif