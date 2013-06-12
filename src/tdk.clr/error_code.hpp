#pragma once

#include <tdk/error/error_code.hpp>

namespace tdk {
namespace clr {

public ref class error_code {
public:
	error_code( void );
	error_code( const tdk::error_code& code );

	System::String^ message( void );

	int value( void );

	bool error( void );
private:
	int _value;
	const tdk::error_category* _category;
};

}}