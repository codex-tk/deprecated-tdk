#ifndef __tdk_tdk_error_castgory_h__
#define __tdk_tdk_error_castgory_h__

//#include <system_error>
#include <tdk/error/error_category.hpp>

namespace tdk {

class tdk_error_category : public tdk::error_category {
public:
	tdk_error_category (void);
	virtual ~tdk_error_category (void) ;
	virtual const char *name() const;
	virtual std::string message(int _Errval) const ;
};

}

#endif