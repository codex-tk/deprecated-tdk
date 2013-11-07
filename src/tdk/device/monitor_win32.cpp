#include "stdafx.h"
#include <tdk/device/monitor_win32.hpp>
#if defined( _WIN32 )
namespace tdk {
namespace device {

monitor::monitor( HMONITOR h ){
	_info.cbSize = sizeof( _info );
	GetMonitorInfo( h , &_info );
}

monitor::~monitor( void ) {

}

tdk::rect monitor::display_area( void ) const {
	return tdk::rect( _info.rcMonitor );
}

tdk::rect monitor::work_area( void ) const {
	return tdk::rect( _info.rcWork );
}

tdk::tstring monitor::device( void ) const {
	return tdk::tstring( _info.szDevice );
}

bool monitor::is_primary( void ) const {
	return _info.dwFlags & MONITORINFOF_PRIMARY;
}

}}
#endif
