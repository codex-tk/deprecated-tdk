#include "stdafx.h"

#include <tdk/error/error_category_tdk.hpp>
#include <tdk/util/string.hpp>

#include <sstream>

namespace tdk {
namespace detail {

	struct tdk_error_table{
		tdk::errc ec;
		const char* messgae;
	};

	static const tdk_error_table tdk_error_tables[] =
	{
		 { errc::tdk_success				, "tdk_success" } 
		,{ errc::tdk_tls_not_initialized	, "tdk_tls_not_initialized" } 
		,{ errc::tdk_user_abort				, "tdk_user_abort" } 
		,{ errc::tdk_network_user_abort		, "tdk_network_user_abort" } 
		,{ errc::tdk_network_remote_closed	, "tdk_network_remote_closed" } 
        ,{ errc::tdk_epoll_error            , "tdk_epoll_error" }
        ,{ errc::tdk_epoll_hang_up          , "tdk_epoll_hang_up" }
		,{ errc::tdk_errc_end				, nullptr }
	};

}

error_category_tdk::error_category_tdk(void) {

}

error_category_tdk::~error_category_tdk(void) _NOEXCEPT {

}

const char *error_category_tdk::name() const _NOEXCEPT {
	return "tdk_category";
}

std::string error_category_tdk::message(int _Errval) const  {
	const detail::tdk_error_table* tab = &detail::tdk_error_tables[0];
	while ( tab->ec != errc::tdk_errc_end ) {
		if ( (int)tab->ec == _Errval ) {
			return tab->messgae;
		}
		++tab;
	}
	return "undefined";
}

}
