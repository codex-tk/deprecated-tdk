#ifndef __tdk_windows_error_castgory_h__
#define __tdk_windows_error_castgory_h__

#include <system_error>

namespace tdk {

class windows_error_category : public std::error_category {
public:
	windows_error_category(void);
	virtual ~windows_error_category(void) ;
	virtual const char *name() const;
	virtual std::string message(int _Errval) const ;
};

}

#endif