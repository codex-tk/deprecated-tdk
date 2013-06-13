#ifndef __tdk_config_registry_h__
#define __tdk_config_registry_h__i

#include <winreg.h>

namespace tdk {
namespace config {

class registry{
public:
	registry( void );
	~registry(void);
	
	bool open( HKEY root , const tdk::tstring& subkey , REGSAM desired = KEY_ALL_ACCESS );
	void close( void );

	bool set_value( const tdk::tstring& key , DWORD value );
	bool get_value( const tdk::tstring& key , DWORD& value );

	bool set_value( const tdk::tstring& key , const tdk::tstring& value );
	bool get_value( const tdk::tstring& key , tdk::tstring& value ); 

	bool delete_value( const tdk::tstring& key );

private:
	HKEY _key;
	HKEY _root;
	tdk::tstring _subkey;
};

}}


#endif