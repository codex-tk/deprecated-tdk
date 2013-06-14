#include "stdafx.h"
#include <tdk/util/win32/registry.hpp>

TEST( reg ,t ) {

	tdk::util::registry reg;
	reg.open( HKEY_CURRENT_USER , _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"));

	reg.set_value( _T("tk_test") , _T("testvalue"));

	reg.set_value( _T("tk_test") , _T("testvalue1"));	

	tdk::tstring value;
	reg.get_value(  _T("tk_test") , value );


	reg.set_value( _T( "tk_dword" ) , 1);

	DWORD vv;

	reg.get_value( _T( "tk_dword" ) , vv );

	reg.delete_value( _T( "tk_dword" ));
	reg.delete_value( _T( "tk_test" ));

	reg.close();

}