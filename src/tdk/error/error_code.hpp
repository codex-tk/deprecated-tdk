#ifndef __tdk_error_code_h__
#define __tdk_error_code_h__

#include <tdk/tdk.hpp>
#include <tdk/error/error_category.hpp>
#include <system_error>

namespace tdk {
/*
	c++11 지원이 나아지면 std::error_code 로 변경할것 
	기존에 사용하던 diagnostics::error 에서 
	std::error_code 와 동일한 형태로 쓸수 잇게 수정한것
*/
	/*
class error_code {
public:
	error_code( void );
	error_code( int ec , const error_category& category );
	error_code( const error_code& rhs );
	error_code operator=( const error_code& rhs );
	~error_code(void);

	tdk::tstring message( void ) const;

	int value( void ) const;
	const error_category& category( void ) const;

	typedef int ( error_code::*unspecfied_bool_type)(void) const;
	operator unspecfied_bool_type() const ;
private:
	int _value;
	const error_category* _category;
};

static bool operator==( const error_code& lhs , const error_code& rhs ) {
	return (lhs.value() == rhs.value()) 
		&& ( &lhs.category() == &rhs.category() );
}

static bool operator!=( const error_code& lhs , const error_code& rhs ) {
	return (lhs.value() != rhs.value()) 
		|| ( &lhs.category() != &rhs.category() );
}
*/
}

#include <tdk/error/last_error.hpp>
#include <tdk/error/error_platform.hpp>
#include <tdk/error/error_tdk.hpp>

#endif