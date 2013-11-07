#ifndef __tdk_device_monitor_h__
#define __tdk_device_monitor_h__
#if defined( _WIN32 )
#include <tdk/geo.hpp>

namespace tdk {
namespace device {

class monitor {
public:
	monitor( HMONITOR h );
	~monitor( void );

	tdk::rect display_area( void ) const;
	tdk::rect work_area( void ) const;

	tdk::tstring device( void ) const;
	bool is_primary( void ) const;
private:
	MONITORINFOEX _info;
};

}}

#endif
#endif
