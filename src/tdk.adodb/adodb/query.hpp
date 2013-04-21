#ifndef __tdk_adodb_query_h__
#define __tdk_adodb_query_h__

#include <tdk.adodb/adodb/connection.hpp>

namespace tdk {
namespace adodb {
struct record_set_impl;
class query {
public:
	/// 생성자
	query();
	/// 소멸자
	~query();

	/// Connection 설정 함수
	bool connection( tdk::adodb::connection& conn );

	// - 커서타입(ADO상수)
	// 
	// 0. <adOpenForwardOnly> : 레코드를 앞으로 순차적으로 한번만 이동
	// 1. <adOpenKeyset> : 레코드 자유이동,레코드갱신,다른사용자가추가한 내용 열람불가
	// 2. <adOpenDynamic> : 레코드 자유이동,레코드갱신,다른사용자가 추가한 내용 열람가능
	// 3. <adOpenStatic> : 레코드 자유이동,레코드갱신불가,단지데이터를 가져오는데유용
	// 
	// -  Lock 타입
	// 1. adLockReadOnly : 읽기
	// 2. adLockRessimistic : 동시갱신 불가.
	// 3. adLockOptimistic : 동시갱신가능하나 update메서드를 실행하면 갱신안됨.
	// 4. adLockBatchOptimistic : 갱신모드로 들어가면 작업가능.
	// 
	// -  옵션
	// 1. adCmdText : 쿼리문
	// 2. adCmdStoredProc : 저장프로시저
	// 3. adCmdTable : 테이블
	//																		- from 지식인
	/// 쿼리 실행 함수 
	//bool execute( const std::string& query , CursorTypeEnum opt1 = adOpenStatic , LockTypeEnum opt2 = adLockBatchOptimistic , CommandTypeEnum opt3 = adCmdText );
	bool execute( const std::string& query );

	/// 레코드의 끝인지 확인
	bool is_eof();
	/// 다음 레코드로 이동
	void next();
	/// 종료
	void close();

	/// 필드의 갯수 알아오기
	int field_count();
	/// 레코드의 갯수 알아오기
	int record_count();
	/// 필드의 값을 Int 로 가져오기
	bool read_int( const std::string& filed_name, int& outInt );
	/// 필드의 값을 String 으로 가져오기
	bool read_string( const std::string& filed_name , std::string& out  );
private:
	record_set_impl* _impl;
};

}}

#endif