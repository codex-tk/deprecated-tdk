#ifndef __ssl_error_category_h__
#define __ssl_error_category_h__

#include <system_error>

namespace tdk {
namespace ssl {
	
class error_category : public std::error_category {
public:
	error_category(void);
	virtual ~error_category(void) _NOEXCEPT  ;

	virtual const char *name(void) const _NOEXCEPT ;
	virtual std::string message(int _Errval) const ;
};

std::error_code ssl_error( int ec );

}}

#endif
