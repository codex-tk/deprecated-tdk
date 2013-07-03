#include "Stdafx.h"
#include "error_code.hpp"

#include <msclr/marshal_cppstd.h>

#include <tdk/error/error_platform.hpp>

using namespace msclr::interop;

namespace tdk {
namespace clr {

error_code::error_code( void )
	: _value( 0 ) , _category(&tdk::platform::category()){

}

error_code::error_code( const std::error_code& code ) 
	: _value( code.value() ) , _category( &code.category() ){

}

System::String^ error_code::message( void ) {
	return gcnew System::String( _category->message(_value).c_str() );
}

int error_code::value( void ){
	return _value;
}

bool error_code::error( void ) {
	return _value != 0;
}

}}