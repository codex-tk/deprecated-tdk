#include "stdafx.h"

#include <tdk.adodb/adodb/connection.hpp>
#include <tdk.adodb/adodb/query.hpp>
/*
TEST(  connection , open ) {
	CoInitialize( nullptr );	
	tdk::adodb::connection connection;
	// install odbc driver http://www.postgresql.org/ftp/odbc/versions/msi/ 
	std::string conn_str( "Driver={PostgreSQL UNICODE};Server=127.0.0.1;Port=5432;Database=tk_test;Uid=postgres;Pwd=1331;" );
	ASSERT_TRUE( connection.open( conn_str ));
	ASSERT_TRUE( connection.is_connected() );
	tdk::adodb::query query;
	query.connection( connection );
	query.execute( "select * from test_table" );
	while( !query.is_eof() ) {
		std::string name;
		query.read_string( "name" , name );
		query.next();
	}
	query.close();
	connection.close();
	CoUninitialize();
}*/