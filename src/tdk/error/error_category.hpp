#ifndef __tdk_error_category_h__
#define __tdk_error_category_h__

#include <string>


namespace tdk {

/*
	c++11 지원이 나아지면 std::error_category 로 변경할것 
*/
class error_category {
public:
	error_category(void);
	virtual ~error_category(void);
	virtual const char *name() const = 0;
	virtual tdk::tstring message(int _Errval) const = 0;

	bool operator==(const error_category& _Right) const {	
		return (this == &_Right);
	}

	bool operator!=(const error_category& _Right) const {	
		return (!(*this == _Right));
	}

	bool operator<(const error_category& _Right) const {	
		return (this < &_Right);
	}
private:
	error_category(const error_category&);	// not defined
	error_category& operator=(const error_category&);	// not defined
};

}

#endif

