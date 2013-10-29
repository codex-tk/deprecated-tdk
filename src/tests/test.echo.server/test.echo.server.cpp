// test.echo.server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

/*
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE 128

int SocketCount = 0;

typedef struct //소켓 정보를 구조체화.
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct // 소켓의 버퍼 정보를 구조체화.
{
	OVERLAPPED overlapped;
	char buffer[BUFSIZE];
	WSABUF wsaBuf;
} PER_IO_DATA, *LPPER_IO_DATA;

DWORD WINAPI CompletionThread(LPVOID CompletionPortIO);
void ErrorHandling(char *message);

int main()
{
	WSADATA	wsaData;
	HANDLE hCompletionPort;	
	SYSTEM_INFO SystemInfo;
	SOCKADDR_IN servAddr;
	LPPER_IO_DATA PerIoData;
	LPPER_HANDLE_DATA PerHandleData;
	
	SOCKET hServSock;
	unsigned long RecvBytes;
	unsigned long i, Flags;
	
	
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!"); 
	
	//1. Completion Port 생성.
	hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&SystemInfo);
	
	//2. Completion Port 에서 입출력 완료를 대기 하는 쓰레드를 CPU 개수 만큼 생성.
	for(i=0; i<SystemInfo.dwNumberOfProcessors; i++)
		CreateThread(NULL, NULL, CompletionThread, (LPVOID)hCompletionPort, 0, NULL);
	
	hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servAddr.sin_port=htons(7543);
	
	bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
	listen(hServSock, 5);
	
	while(TRUE)
	{		
		SOCKET hClntSock;
		SOCKADDR_IN clntAddr;		
		int addrLen=sizeof(clntAddr);
		
		hClntSock=accept(hServSock, (SOCKADDR*)&clntAddr, &addrLen);	
		printf("소켓이 %d개 연결됨 \r",++SocketCount);
		
		// 연결 된 클라이언트의 소켓 핸들 정보와 주소 정보를 설정.
		PerHandleData=(LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));		
		PerHandleData->hClntSock=hClntSock;
		memcpy(&(PerHandleData->clntAddr), &clntAddr, addrLen);
		
		//3. Overlapped 소켓과 CompletionPort의 연결.
		CreateIoCompletionPort((HANDLE)hClntSock, hCompletionPort, (DWORD)PerHandleData, 0);
		
		// 연결 된 클라이언트를 위한 버퍼를 설정 하고 OVERLAPPED 구조체 변수 초기화.
		PerIoData=(LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));		
		PerIoData->wsaBuf.len=BUFSIZE;
		PerIoData->wsaBuf.buf=PerIoData->buffer;
		Flags=0;
		
		//4. 중첩된 데이터 입력.
		WSARecv(PerHandleData->hClntSock,	// 데이터 입력 소켓.
			&(PerIoData->wsaBuf),		// 데이터 입력 버퍼 포인터.
			1,							// 데이터 입력 버퍼의 수.
			&RecvBytes,					
			&Flags, 
			&(PerIoData->overlapped),	// OVERLAPPED 구조체 포인터.
			NULL
			);			
	}
	return 0;
}

//입 출력 완료에 따른 쓰레드의 행동 정의 
DWORD WINAPI CompletionThread(LPVOID pComPort)
{
	HANDLE hCompletionPort =(HANDLE)pComPort;
	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_DATA PerIoData;
	DWORD flags;
	
	while(1)
	{
		// 5. 입출력이 완료 된 소켓의 정보 얻음. 
		GetQueuedCompletionStatus(hCompletionPort,			 // Completion Port
			&BytesTransferred,		 // 전송 된 바이트 수
			(LPDWORD)&PerHandleData,
			(LPOVERLAPPED*)&PerIoData, // OVERLAPPED 구조체 포인터.
			INFINITE
			);
		
		if(BytesTransferred==0) //EOF 전송시.
		{
			closesocket(PerHandleData->hClntSock);
			SocketCount--;
			free(PerHandleData);
			free(PerIoData);
			continue;		
		}		
		
		// 6. 메시지! 클라이언트로 에코.
		PerIoData->wsaBuf.len=BytesTransferred;
		WSASend(PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, NULL, 0, NULL, NULL);
		
		// RECEIVE AGAIN
		memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));
		PerIoData->wsaBuf.len=BUFSIZE;
		PerIoData->wsaBuf.buf=PerIoData->buffer;
		
		flags=0;
		WSARecv(PerHandleData->hClntSock, 
			&(PerIoData->wsaBuf), 
			1, 
			NULL, 
			&flags, 
			&(PerIoData->overlapped), 
			NULL
			);							
	}
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
*/


#include <tdk/tdk.hpp>
#include <tdk/io/ip/tcp/pipeline/filter.hpp>
#include <tdk/io/ip/tcp/pipeline/pipeline_builder.hpp>
#include <tdk/io/ip/tcp/filters/recv_time_out_filter.hpp>
#include <tdk/io/ip/tcp/channel_acceptor.hpp>
#include <tdk/threading/thread_data.hpp>
#include <vector>
#include <atomic>

std::atomic< int > all_connected;
std::atomic< int > current_conns;
std::atomic< int > read_bytes;
std::atomic< int > write_bytes;

class echo_handler : public tdk::io::ip::tcp::filter {
public:
	echo_handler( void ) {

	}
	virtual ~echo_handler( void ) {
	}

	virtual void on_accepted( const tdk::io::ip::address& addr ) {
		++all_connected;
		++current_conns;
	}
	virtual void on_error( const std::error_code& ec ) {
		channel()->close();
	}

	virtual void on_read( tdk::buffer::memory_block& msg ) {
		read_bytes += msg.length();
		write_out_bound( msg );
	}

	virtual void on_write( int w , bool flushed ) {
		write_bytes += w;
	}

	virtual void on_closed( void ) {
		--current_conns;
	}
};

class echo_builder : public tdk::io::ip::tcp::pipeline_builder{
public:
	echo_builder( void ) {		
	}
	virtual std::error_code build( tdk::io::ip::tcp::pipeline& p ) {
		p.add( "timeout" 
			,  new tdk::io::ip::tcp::recv_time_out_filter(
					tdk::time_span::from_seconds(10)));
		p.add( "echo" , new echo_handler()  );
		return std::error_code();
	}
};


class acceptor : public tdk::io::ip::tcp::channel_acceptor {
public:
	acceptor( tdk::event_loop& l ) 
		: channel_acceptor(l)
	{
		_index = 0;
		for ( int i = 0 ; i < 4 ; ++i ) {
			tdk::event_loop* l = new tdk::event_loop();
			std::thread( [l] {
				tdk::threading::data td;
				tdk::threading::this_thread::set_data( &td );
				l->add_active();
				l->run();
			}).detach();
			_loops.push_back( l );
		}
		
	}

	virtual tdk::event_loop& channel_loop( void ) {
		if ( _index >= _loops.size() ) {
			_index = 0;
		}
		return *_loops[_index++];
	}
private:
	int _index;
	std::vector<tdk::event_loop*> _loops;
};

void on_timer( tdk::task* t ) {
	printf( "Current %d / %d   R: %d    W: %d\n" , current_conns.load()
		, all_connected.load()
		, read_bytes.load()
		, write_bytes.load());
	tdk::timer_task* tt = static_cast< tdk::timer_task* >(t);
	tt->expired_at( tdk::date_time::utc() + tdk::time_span::from_seconds(5));
	tdk::event_loop* l = static_cast< tdk::event_loop* >(t->context());
	l->schedule( tt );
}

int _tmain(int argc, _TCHAR* argv[])
{
	tdk::init();

	current_conns.store(0);
	all_connected.store(0);
	read_bytes.store(0);
	write_bytes.store(0);
	echo_builder b;
	tdk::event_loop l;

	tdk::timer_task print_timer( &on_timer  , &l );
	print_timer.expired_at( tdk::date_time::utc() + tdk::time_span::from_seconds(5));
	l.schedule( &print_timer );
	acceptor a(l);
	if ( a.open(tdk::io::ip::address::any( 7543 ) , &b )) {
		l.run();
	}
	return 0;
}

