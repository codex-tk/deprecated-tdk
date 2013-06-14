#ifndef __tdk_win32_service_manager_h__
#define __tdk_win32_service_manager_h__

namespace tdk {
namespace util {
	
class service;
class service_manager {
public:
	service_manager( void );
	~service_manager( void );

	bool install( const service& svc );
	bool uninstall( const service& svc );
private:
	SC_HANDLE _handle;
};

}}

#endif