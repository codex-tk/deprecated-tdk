#include "stdafx.h"
#include <tdk/device/display_win32.hpp>


namespace tdk {
namespace device {
namespace detail {

std::vector< HMONITOR > monitors;
BOOL CALLBACK MONITORENUMPROC_IMPL(HMONITOR hm, HDC dc  , LPRECT r, LPARAM p ) {
	monitors.push_back( hm );
	return TRUE;
}

}


display::display( void ) {
	update();
}

display::~display( void ) {

}

bool display::update( void ) {
	_size.width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	_size.height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	detail::monitors.clear();
	EnumDisplayMonitors( NULL , NULL , &detail::MONITORENUMPROC_IMPL , NULL );
	_monitors.clear();

	for( HMONITOR h : detail::monitors ) {
		_monitors.push_back( tdk::device::monitor( h ));
	}
	return _monitors.size() != 0;
}

const std::vector< tdk::device::monitor >& display::monitors( void ) {
	return _monitors;
}

tdk::size display::size( void ) {
	return _size;
}
}}