#ifndef __tdk_adodb_connection_h__
#define __tdk_adodb_connection_h__

namespace tdk {
namespace adodb {
struct connection_impl;
class connection {
public:
	/// 생성자 
	connection();
	/// 소멸자
	~connection();

	/* postgresql */
	// install odbc driver http://www.postgresql.org/ftp/odbc/versions/msi/ 
	// "Driver={PostgreSQL UNICODE};Server=127.0.0.1;Port=5432;Database=tk_test;Uid=postgres;Pwd=1331;"

	/* mssql */
	//L"Provider=SQLOLEDB.1;Persist Security Info=True;Password = kttcop;User ID = kttddns;Initial Catalog = KTT_DDNS;Data Source = 172.16.71.200";
	bool open( const std::string& connstr );

	void close();

	/// ADOQuery 등에서 사용하는 Connection 객체를 가져오는 함수
	connection_impl* value();
		
	/// 연결이 설정돼어 있는지 확인하는 함수
	bool is_connected();

private:
	connection_impl* _impl;
};

}}
#endif