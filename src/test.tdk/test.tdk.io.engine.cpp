#include "stdafx.h"
#include <tdk/io/engine_win32.hpp>
#include <tdk/io/ip/tcp/socket_win32.hpp>

class test_executor {
public:
	template < typename T_handler >
	tdk::io::detail::wrapped_handler< T_handler , test_executor > wrap( const T_handler& h ) {
		return tdk::io::detail::wrapped_handler< T_handler , test_executor >( h , this );
	}
	void execute( tdk::io::operation* op
		, void (__stdcall* do_dispatch )( tdk::io::operation* ) 
		, void (__stdcall* do_delete )( tdk::io::operation* ) )
	{
		do_dispatch( op );
		do_delete( op );
	}

};

TEST( tdk_io_engine , e ){
	tdk::io::engine e;
	ASSERT_TRUE(e.open());

	test_executor executor;

	tdk::io::ip::tcp::socket fd( e );

	std::vector< tdk::io::ip::address > addres;
	addres.push_back( tdk::io::ip::address( "127.0.0.1" , 80 )); 
	tdk::io::ip::address::resolve( "google.co.kr" , 80 , addres );
	bool end = false;
	fd.async_connect( addres 
		, executor.wrap(
			[&]( const tdk::error_code& e ){
			OutputDebugStringW( e.message().c_str());
			end = true;
		}));

	while ( !end ) 
		e.run( tdk::time_span::infinite());

	end = false;

	char* get = "GET / HTTP/1.1\r\n\r\n";
	tdk::io::buffer_adapter buffer( get , strlen(get));
	fd.async_send( buffer , [&] ( const tdk::error_code& e , int io_byte ) {
		OutputDebugStringW( e.message().c_str());
		end = true;
	});

	while( !end ) {
		e.run( tdk::time_span::infinite());
	}

	end = false;

	char recv_buffer[4096];
	tdk::io::buffer_adapter recv_adapter( recv_buffer , 4096 );

	fd.async_recv(recv_adapter , [&]( const tdk::error_code& e , int io_byte ) {
		OutputDebugStringW( e.message().c_str());
		end = true;
	});

	while( !end ) {
		e.run( tdk::time_span::infinite());
	}
	fd.close();
	e.close();
}
/*

TEST( tdk_io_ip_tcp_accept , t1 ) {
	tdk::io::engine e;
	ASSERT_TRUE(e.open());

	tdk::io::ip::tcp::socket acceptor(  e );
	tdk::io::ip::tcp::socket session( e );

	ASSERT_TRUE(acceptor.open_tcp( AF_INET ));
	ASSERT_TRUE(acceptor.bind( tdk::io::ip::address::any( 7543 )));
	ASSERT_TRUE(acceptor.listen());

	ASSERT_TRUE( e.bind( acceptor.handle() ));
	bool end = false;
	acceptor.async_accept( session , [&]( const tdk::error_code& e ) {
			OutputDebugString( _T("Accept Complete : "));
			OutputDebugString( e.message().c_str ());
			end = true;
		});
	while( !end ) {
		e.run( tdk::time_span::infinite());
	}
}*/