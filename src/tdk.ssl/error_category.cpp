#include "stdafx.h"

#include "error_category.hpp"
#include <openssl/err.h>

#include <sstream>

namespace tdk {
namespace ssl {

error_category::error_category(void) {

}

error_category::~error_category(void) _NOEXCEPT {

}

const char *error_category::name() const _NOEXCEPT  {
	return "ssl_error";
}

std::string error_category::message(int _Errval) const  {
	char err_msg[4096];
	ERR_error_string( _Errval , err_msg );
	return std::string( err_msg );
}

std::error_code ssl_error( int ec ) {
	static ssl::error_category ssl_category;
	return std::error_code( ec , ssl_category );
}

}}

