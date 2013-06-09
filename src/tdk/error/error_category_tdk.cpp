#include "stdafx.h"

#include <tdk/error/error_category_tdk.hpp>
#include <tdk/util/string.hpp>

#include <sstream>

namespace tdk {

error_category_tdk::error_category_tdk(void) {

}

error_category_tdk::~error_category_tdk(void) {

}

const TCHAR* error_category_tdk::name() const {
	return _T("error_category_tdk");
}

tdk::tstring error_category_tdk::message(int ec) const  {
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