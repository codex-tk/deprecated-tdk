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

tdk::tstring tdk_error_category::message(int ec) const  {
	switch ( ec ) {
	case errc::tdk_success:
		return L"tdk_success";
	case errc::tdk_tls_not_initialized:
		return L"tdk_tls_not_initialized";
	case errc::tdk_network_user_abort:
		return L"tdk_network_user_abort";
	case errc::tdk_network_remote_closed:
		return L"tdk_network_remote_closed";
	}
	return tdk::tstring(L"undefined");
}

}