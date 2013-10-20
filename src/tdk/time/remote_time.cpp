/*
 * remote_time.cpp
 *
 *  Created on: 2013. 10. 17.
 *      Author: tk
 */
#include "stdafx.h"
#include <tdk/time/remote_time.hpp>

namespace tdk {

remote_time::remote_time() {
	// TODO Auto-generated constructor stub

}

remote_time::~remote_time() {
	// TODO Auto-generated destructor stub
}

void remote_time::register_time( const std::string& name , const tdk::time_span& diff) {
	_remotes[name] = diff;
}

tdk::time_span remote_time::diff( const std::string& name ) {
	auto it = _remotes.find(name);
	if ( it != _remotes.end()) {
		return it->second;
	}
	return tdk::time_span(0);
}

} /* namespace tdk */
