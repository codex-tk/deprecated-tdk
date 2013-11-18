// test.iocp.post.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <atomic>
#include <thread>
#include <chrono>
#include <ctime>
#include "list_node.hpp"

HANDLE _iocp;
std::atomic<int> _runs;
std::atomic<int> _posted;
int post_count = 200000;
int post_thread_count = 16;
spin_lock* _lock;
tdk::slist_queue< task > _queue;

class task;

void run( task* t ) ;

void post_thread_0( void ) {
	for ( int i = 0 ; i < post_count ; ++i ) {
		task* pt = new task( &run );
		PostQueuedCompletionStatus( _iocp 
			, 0
			, reinterpret_cast< ULONG_PTR >( pt ) 
			, nullptr );
	}
}

void process_0( void ){
	_runs.store(0);

	LPOVERLAPPED overlapped = nullptr;
    ULONG_PTR key = 0;
    DWORD bytes_transferred = 0;

	for ( int i = 0 ; i < post_thread_count ; ++i ) {
		std::thread([]{
			post_thread_0();
		}).detach();
	}

	while ( _runs.load() < post_count * post_thread_count ) {
		GetQueuedCompletionStatus( 
			_iocp
			, &bytes_transferred 
			, &key
			, &overlapped
			, 1000000000 );
		task* t = reinterpret_cast<task*>(key);
		(*t)();
		delete t;
	}
}

void post_thread_1( void ) {
	for ( int i = 0 ; i < post_count ; ++i ) {
		task* pt = new task( &run );
		{
			_lock->lock();
			_queue.add_tail( pt );
			_lock->unlock();
		}
		int expected = 0;
		if ( _posted.compare_exchange_strong( expected , 1 )) {
			if ( PostQueuedCompletionStatus( _iocp , 0 , 0 , 0 ) == FALSE ) {
				_posted.exchange(0);
			}
		}
	}
}

void process_1( void ){
	_runs.store(0);

	LPOVERLAPPED overlapped = nullptr;
    ULONG_PTR key = 0;
    DWORD bytes_transferred = 0;

	for ( int i = 0 ; i < post_thread_count ; ++i ) {
		std::thread([]{
			post_thread_1();
		}).detach();
	}

	while ( _runs.load() < post_count * post_thread_count ) {
		GetQueuedCompletionStatus( 
			_iocp
			, &bytes_transferred 
			, &key
			, &overlapped
			, 1000000000 );
		_posted.exchange(0);

		tdk::slist_queue< task > queue;
		{
			_lock->lock();
			queue.add_tail( _queue );
			_lock->unlock();
		}
		while ( !queue.is_empty() ) {
			task* t = queue.front();
			queue.pop_front();
			(*t)();
			delete t;
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	_lock = new spin_lock();	
	_posted.store(0);
	_iocp = CreateIoCompletionPort( INVALID_HANDLE_VALUE , NULL , NULL , 0 );

	std::cout << "start" << std::endl;
	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::chrono::duration<double> elapsed_seconds;

	start = std::chrono::system_clock::now();    

	process_1();
	
	end = std::chrono::system_clock::now();
 
    elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
	start = std::chrono::system_clock::now();    

	process_0();

	end = std::chrono::system_clock::now(); 
    elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

	return 0;
}



void run( task* t ) {
	_runs.fetch_add(1);
}