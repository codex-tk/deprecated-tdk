#ifndef __tdk_device_display_h__
#define __tdk_device_display_h__
#if defined( _WIN32 )
#include <tdk/device/monitor_win32.hpp>
#include <vector>

namespace tdk {
namespace device {

class display {
public:
	display( void );
	~display( void );

	bool update( void );

	const std::vector< tdk::device::monitor >& monitors( void );

	tdk::size size( void );
private:
	std::vector< tdk::device::monitor > _monitors;
	tdk::size _size;
};

}}
#endif
#endif
