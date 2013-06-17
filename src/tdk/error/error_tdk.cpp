#include "stdafx.h"
#include <tdk/error/error_tdk.hpp>

namespace tdk {
	
const tdk::error_category& category( void ) {
	static tdk::error_category_tdk impl;
	return impl;
}

tdk::error_code error( tdk::errc ec ){
	return tdk::error_code( (int)ec , tdk::category());
}

 tdk::error_code tdk_success( (int)errc::tdk_success , tdk::category() );
 tdk::error_code tdk_tls_not_initialized( (int)errc::tdk_tls_not_initialized , tdk::category() );
 tdk::error_code tdk_user_abort( (int)errc::tdk_user_abort , tdk::category() );
 tdk::error_code tdk_network_user_abort( (int)errc::tdk_network_user_abort , tdk::category() );
 tdk::error_code tdk_network_remote_closed( (int)errc::tdk_network_remote_closed , tdk::category() );


}

