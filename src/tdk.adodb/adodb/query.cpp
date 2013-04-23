#include "stdafx.h"
#include <tdk.adodb/adodb/query.hpp>
#include <iostream>
#include "connection_impl.hpp"

namespace tdk {
namespace adodb {

query::query()
	: _impl( new record_set_impl ())
{
	_impl->record_set_ptr.CreateInstance( __uuidof( Recordset ));
}
query::~query()
{
	close();
	_impl->record_set_ptr = NULL;
	delete _impl;
}
bool query::connection( tdk::adodb::connection& conn  )
{
	try
	{
		_impl->connection_ptr = conn.value()->connection_ptr;
		_impl->record_set_ptr->putref_ActiveConnection( _impl->connection_ptr );
	}
	catch (_com_error& err)
	{
		(void)err;
		return false;
	}
	return true;
}

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

/*
bool query::execute( const std::string& query 
	, CursorTypeEnum opt1 
	, LockTypeEnum opt2 
	, CommandTypeEnum opt3 )
	*/
bool query::execute( const std::string& query ) {
	_variant_t vt_conn( (IDispatch*) _impl->connection_ptr );
	_variant_t vt_query( query.c_str());
	try
	{
		close();
		_impl->record_set_ptr->Open(vt_query, vt_conn
			, adOpenStatic
			, adLockBatchOptimistic
			, adCmdText );
		//_record_set->MoveFirst();
	}
	catch (_com_error& err)
	{
		(void)err;
		return false;
	}
	return true;
}

bool query::is_eof()
{
	try
	{
		if( _impl->record_set_ptr == NULL )
			return true;
		if( _impl->record_set_ptr->adoEOF == VARIANT_FALSE )
			return false;
		else 
			return true;
	}
	catch (_com_error& err)
	{
		(void)err;
		return true;
	}
	return true;
}

void query::next()
{
	try
	{
		_impl->record_set_ptr->MoveNext();
	}
	catch (_com_error& err)
	{
		(void)err;
	}
}
void query::close()
{
	try
	{
		if( _impl->record_set_ptr->GetState() == adStateOpen )	
			_impl->record_set_ptr->Close();
	}
	catch (_com_error& err)
	{
		(void)err;
	}
		
}

int query::field_count()
{
	try
	{
		return (int)_impl->record_set_ptr->Fields->GetCount();
	}
	catch (_com_error& err)
	{
		(void)err;
		return 0;
	}
		
}
int query::record_count()
{
	try
	{
		return (int)_impl->record_set_ptr->GetRecordCount();
	}
	catch (_com_error& err)
	{
		(void)err;
		return 0;
	}
}	


bool query::read_int( const std::string& filed_name, int& outInt  )
{
	//_variant_t fieldIndex( index );
	try
	{
		outInt = _impl->record_set_ptr->Fields->GetItem(filed_name.c_str())->Value.intVal;
		//*outInt = _record_set->Fields->Item[fieldIndex]->Value.intVal;
	}
	catch (_com_error& err)
	{
		(void)err;
		std::cout << err.ErrorMessage() << std::endl;
		return false;
	}
	return true;
}

bool query::read_string( const std::string& filed_name , std::string& out  )
{
	//	_variant_t fieldIndex( index );
	try
	{
		_variant_t   variant(DISP_E_PARAMNOTFOUND, VT_ERROR);

		variant = _impl->record_set_ptr->Fields->GetItem(filed_name.c_str())->Value;

		if (variant.vt != VT_NULL){
			out = (LPCSTR) _bstr_t(variant.bstrVal);
			//strncpy_s( outbuf , outbufSize - 1 ,  (LPCSTR) _bstr_t(variant.bstrVal) , outbufSize - 1 );
		}else
			return false;
	}
	catch (_com_error& err)
	{
		(void)err;
		return false;
	}
	return true;
}

}}