#ifndef __tdk_error_category_win32_h__
#define __tdk_error_category_win32_h__

#include <system_error>
#include <tdk/tdk.hpp>
//#include <tdk/error/error_category.hpp>

namespace tdk {
namespace platform {
	
class error_category_platform : public std::error_category {
public:
	error_category_platform(void);
	virtual ~error_category_platform(void) _NOEXCEPT  ;
    

	virtual const char *name(void) const _NOEXCEPT ;
	virtual std::string message(int _Errval) const ;
	/*
	virtual std::error_condition
		default_error_condition(int _Errval) const _NOEXCEPT;

	virtual bool equivalent(int _Errval,
		const std::error_condition& _Cond) const _NOEXCEPT;

	virtual bool equivalent(const std::error_code& _Code,
		int _Errval) const _NOEXCEPT;*/
};

	/*
class error_category_platform : public tdk::error_category {
public:
	error_category_platform(void);
	virtual ~error_category_platform(void) ;
	virtual const TCHAR *name() const;
	virtual tdk::tstring message(int _Errval) const ;
};
*/
}}

#endif
