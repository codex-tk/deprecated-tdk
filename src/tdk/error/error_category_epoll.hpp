#ifndef __tdk_error_category_epoll_h__
#define __tdk_error_category_epoll_h__

#include <tdk/tdk.hpp>

namespace tdk {

class error_category_epoll : public std::error_category {
public:
	error_category_epoll(void);
	virtual ~error_category_epoll(void) _NOEXCEPT ;

	virtual const char *name() const _NOEXCEPT ;
	virtual std::string message(int _Errval) const ;
};
/*
class error_category_tdk : public tdk::error_category {
public:
	error_category_tdk (void);
	virtual ~error_category_tdk (void) ;
	virtual const TCHAR *name() const;
	virtual tdk::tstring message(int _Errval) const ;
};
*/
}

#endif
