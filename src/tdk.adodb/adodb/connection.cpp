#include "stdafx.h"
#include <tdk.adodb/adodb/connection.hpp>
#include "connection_impl.hpp"

namespace tdk {
namespace adodb {
/// 생성자 
connection::connection() 
	: _impl( new connection_impl())
{
	_impl->connection_ptr.CreateInstance( L"ADODB.Connection" );
}
/// 소멸자
connection::~connection(){
	_impl->connection_ptr = nullptr;
	delete _impl;
}

//L"Provider=SQLOLEDB.1;Persist Security Info=True;Password = kttcop;User ID = kttddns;Initial Catalog = KTT_DDNS;Data Source = 172.16.71.200";
/// \brief DB 와의 연결을 설정하기 위한 함수
bool connection::open( const std::string& connstr){
	try {
		_bstr_t conn_bstr( connstr.c_str() );
		_impl->connection_ptr->Open( conn_bstr , "" , "", adCmdUnspecified );
	} catch (_com_error& err) {
		(void)err;
		return false;
	}
	return is_connected();
}
/// \brief DB 와의 연결 종료
void connection::close(){
	try {
		if( is_connected() )
			_impl->connection_ptr->Close();
	} catch (_com_error& err) {
		(void)err;
	}
}

/// ADOQuery 등에서 사용하는 Connection 객체를 가져오는 함수
connection_impl* connection::value(){
	return _impl;
}
		
/// 연결이 설정돼어 있는지 확인하는 함수
bool connection::is_connected(){
	return _impl->connection_ptr->GetState() == adStateOpen;
}

}}