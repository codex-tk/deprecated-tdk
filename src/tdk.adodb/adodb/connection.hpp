#ifndef __tdk_adodb_connection_h__
#define __tdk_adodb_connection_h__

#import "c:/Program Files/Common Files/System/ado/msado15.dll" no_namespace rename("EOF", "adoEOF")

namespace tdk {
namespace adodb {

class connection {

public:
	/// 생성자 
	connection();
	/// 소멸자
	~connection();

	//L"Provider=SQLOLEDB.1;Persist Security Info=True;Password = kttcop;User ID = kttddns;Initial Catalog = KTT_DDNS;Data Source = 172.16.71.200";
	/// \brief DB 와의 연결을 설정하기 위한 함수
	bool open( const char* connstr );
	/// \brief DB 와의 연결 종료
	void close();

	/// ADOQuery 등에서 사용하는 Connection 객체를 가져오는 함수
	_ConnectionPtr value();
		
	/// 연결이 설정돼어 있는지 확인하는 함수
	bool is_connected();

private:
	 _ConnectionPtr _connection;
};

}}
#endif