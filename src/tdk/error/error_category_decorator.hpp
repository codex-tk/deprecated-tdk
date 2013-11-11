#ifndef __tdk_error_category_decorator_h__
#define __tdk_error_category_decorator_h__

#include <system_error>
#include <tdk/tdk.hpp>

namespace tdk {
	
class error_category_decorator : public std::error_category {
public:
	error_category_decorator( const std::string& deco , const std::error_category& base );
	virtual ~error_category_decorator(void) _NOEXCEPT  ;
    

	virtual const char *name(void) const _NOEXCEPT ;
	virtual std::string message(int _Errval) const ;
private:
	const std::error_category& _base_category;
	std::string _deco;
};
}

#endif
