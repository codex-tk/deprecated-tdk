#include "stdafx.h"
#include <tdk/service/service.hpp>
#include <tdk/threading/spin_lock.hpp>

namespace tdk {
namespace win32 {
namespace {

tdk::threading::spin_lock _map_lock;
std::map< std::wstring , service* > _service_map;

}
	
service::service( const std::wstring& svc_name ) 
: _name(svc_name)
,_desire_access(SERVICE_ALL_ACCESS) 
,_service_type(SERVICE_WIN32_OWN_PROCESS)
,_start_type(SERVICE_AUTO_START)
,_error_control(SERVICE_ERROR_IGNORE)
,_path( _T("") )
{
	TCHAR path[MAX_PATH];
	GetModuleFileName( nullptr , path , MAX_PATH );
	_path = tdk::tstring( path );
}

service::~service( void ) {

}

bool service::start( const service_main_handler& main_handler , const service_event_handler& event_hander ) {
	SERVICE_TABLE_ENTRY stbl[] = {
		{(wchar_t*)_name.c_str(), reinterpret_cast<LPSERVICE_MAIN_FUNCTION>( &service::_service_main )},
        {NULL, NULL}
    };

	_main_handler = main_handler;
	_event_handler= event_hander;

	_service_map.insert( std::map< std::wstring , service* >::value_type( _name , this ));

	if ( StartServiceCtrlDispatcher( stbl ) == TRUE )
		return true;
	return false;
}

void service::run( void ) {
	_status_handle = RegisterServiceCtrlHandlerEx( 
		 _name.c_str()
		,&service::_service_handler
		,this);
	if ( _main_handler.is_good () )
		_main_handler( this );
}

DWORD service::handle_event( DWORD fdwControl, DWORD dwEventType, LPVOID lpEventData ) {
	if ( _event_handler.is_good() ) 
		return _event_handler( this , fdwControl , dwEventType , lpEventData );
	return 0;
}

void service::change_status( DWORD state ) {
    SERVICE_STATUS srvStatus;
    srvStatus.dwServiceType  = SERVICE_WIN32_OWN_PROCESS;
    srvStatus.dwCurrentState = _current_status = state;
    srvStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP |
                SERVICE_ACCEPT_PAUSE_CONTINUE |
                SERVICE_ACCEPT_SHUTDOWN;
    srvStatus.dwWin32ExitCode = 0;
    srvStatus.dwServiceSpecificExitCode = 0;
    srvStatus.dwCheckPoint = 0;
    srvStatus.dwWaitHint = 5000;
    SetServiceStatus(_status_handle, &srvStatus);
}

void  __stdcall service::_service_main( int args , wchar_t** argv ) {
	auto it = _service_map.find( argv[0] );
	if ( it != _service_map.end() ) {
		it->second->run();
	}
}

DWORD __stdcall service::_service_handler(DWORD fdwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext) {
	service* svc = static_cast< service*> ( lpContext );
	return svc->handle_event( fdwControl , dwEventType , lpEventData );
}


}}