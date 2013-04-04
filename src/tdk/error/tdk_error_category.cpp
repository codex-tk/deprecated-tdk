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
	return std::string("");
}

}