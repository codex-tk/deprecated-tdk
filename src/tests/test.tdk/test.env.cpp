#include "stdafx.h"
#include <tdk/util/win32/env.hpp>

TEST( evn , t1 ) {
	
	tdk::tstring osver = tdk::util::env::os_version_string();
	return;
}