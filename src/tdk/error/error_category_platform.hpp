#ifndef __tdk_error_category_win32_h__
#define __tdk_error_category_win32_h__

#include <system_error>
#include <tdk/error/error_category.hpp>
namespace tdk {

class error_category_platform : public tdk::error_category {
public:
	error_category_platform(void);
	virtual ~error_category_platform(void) ;
	virtual const TCHAR *name() const;
	// mbs
	virtual tdk::tstring message(int _Errval) const ;
};

}

#endif