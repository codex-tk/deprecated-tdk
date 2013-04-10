#include "stdafx.h"

#include <tdk/error/tdk_error_category.hpp>
#include <tdk/util/string.hpp>

#include <sstream>

namespace tdk {

tdk_error_category::tdk_error_category(void) {

}

tdk_error_category::~tdk_error_category(void) {

}

const char* tdk_error_category::name() const {
	return "tdk_error_category";
}

std::string tdk_error_category::message(int ec) const  {
	switch ( ec ) {
	case errc::tdk_success:
		return "tdk_success";
	case errc::tdk_tls_not_initialized:
		return "tdk_tls_not_initialized";
	}
	return std::string("undefined");
}

}