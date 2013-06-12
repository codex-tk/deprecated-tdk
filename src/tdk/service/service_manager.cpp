#include "stdafx.h"
#include <tdk/service/service_manager.hpp>
#include <tdk/service/service.hpp>

namespace tdk {
namespace win32 {
	
service_manager::service_manager( void ) 
{

}

service_manager::~service_manager( void ) {
	CloseServiceHandle(_handle);
}

bool service_manager::install( const service& svc ) {
	_handle = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if ( _handle == nullptr ) 
		return false;

	SC_HANDLE svc_handle = CreateService( 
		 _handle
		,svc.name().c_str() 
		,svc.display_name().c_str()
		,svc.desire_access() 
		,svc.service_type()
		,svc.start_type()
		,svc.error_control()
		,svc.path().c_str()
		,nullptr
		,nullptr
		,nullptr
		,nullptr
		,nullptr);

	if ( svc_handle == nullptr ) {
		return false;
	}


	SERVICE_DESCRIPTION desc;
	desc.lpDescription = (LPWSTR)svc.description().c_str();
	
	ChangeServiceConfig2( svc_handle , SERVICE_CONFIG_DESCRIPTION , &desc );
	CloseServiceHandle( svc_handle );

	return true;
}

bool service_manager::uninstall( const service& svc ) {
	_handle = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if ( _handle == nullptr ) 
		return false;

	SC_HANDLE svc_handle = OpenService( _handle , svc.name().c_str() , SERVICE_ALL_ACCESS );

	if ( svc_handle == nullptr ) 
		return false;	
	SERVICE_STATUS ss;
	bool req_stop = false;
	while ( QueryServiceStatus( svc_handle , &ss ) == TRUE ) {
		if ( ss.dwCurrentState == SERVICE_STOPPED ) {
			break;
		} else {
			if ( !req_stop ) {
				ControlService( svc_handle , SERVICE_CONTROL_STOP  , &ss );
				req_stop = true;
			}
		}
		Sleep(1000);
	}

	if ( DeleteService( svc_handle ) == FALSE ) {
		return false;
	}
	return true;
}

}}
