#include "stdafx.h"
#include <tdk/util/win32/registry.hpp>

namespace tdk {
namespace util {

registry::registry( void ) {
	
}

registry::~registry(void) {
}

bool registry::open( HKEY root , const tdk::tstring& subkey  ) {
//bool registry::open(HKEY root , const tdk::tstring& subkey , REGSAM desired){
	_root = root;
	_subkey = subkey;
	/*
	int ret = RegOpenKeyEx( root 
		, subkey.c_str() 
		, 0 
		, desired 
		, &_key );
		*/
	int ret = RegOpenKey( root , subkey.c_str() , &_key );
	if ( ret !=  ERROR_SUCCESS ){
		tdk::set_last_error( tdk::platform::error( ret ));
		return false;
	}
	return true;
}

void registry::close(void){
	RegCloseKey( _key );
}

bool registry::set_value( const tdk::tstring& key , DWORD value ) {
	return RegSetValueEx( _key
		, key.c_str()
		, 0 
		, REG_DWORD  
		, (BYTE*)&value 
		, sizeof( DWORD) ) == ERROR_SUCCESS;
}

bool registry::get_value( const tdk::tstring& key , DWORD& value ) {
	DWORD value_size = sizeof( DWORD );
	if ( RegGetValue( _root 
		, _subkey.c_str()
		, key.c_str()
		, RRF_RT_REG_DWORD 
		, nullptr 
		, &value 
		, &value_size ) == ERROR_SUCCESS )
	{
		return true;
	}
	return false;
}

bool registry::set_value( const tdk::tstring& key , const tdk::tstring& value ) {
	return RegSetValueEx( _key 
		, key.c_str() 
		, 0 , REG_SZ
		, (BYTE*)value.data()
		, static_cast<DWORD>(value.length() * sizeof(TCHAR))) == ERROR_SUCCESS;
}

bool registry::get_value( const tdk::tstring& key , tdk::tstring& value ) {
	TCHAR buffer[4096] = {0,};
	DWORD buffer_size = 4096 * sizeof(TCHAR);
	if ( RegGetValue( _root 
		, _subkey.c_str()
		, key.c_str() 
		, RRF_RT_REG_SZ 
		, nullptr 
		, buffer
		, &buffer_size ) == ERROR_SUCCESS )
	{
		value = buffer;
		return true;
	}
	return false;
}

bool registry::delete_value( const tdk::tstring& key ) {
	return RegDeleteValue( _key , key.c_str()) == ERROR_SUCCESS;
}

}}



