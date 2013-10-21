/*
 * event_loop.cpp
 *
 *  Created on: 2013. 10. 8.
 *      Author: tk
 */

#include <gtest/gtest.h>
#include <tdk/event_loop/event_loop.hpp>

void run_task_handler(tdk::task* t) {
	*static_cast< int* >( t->context()) = 10;
}

TEST( tdk__event_loop , execute ) {
	tdk::event_loop loop;
	int test_value = 5;
	tdk::task run_task( &run_task_handler , &test_value);
	loop.execute( &run_task );
	loop.run();
	ASSERT_EQ( test_value , 10 );
}

void timer_called(tdk::task* t ) {
	*static_cast< int* >( t->context()) = 1;
}

void thread_execute_called( tdk::task* t ) {
	*static_cast< int* >( t->context()) = 1;
}

TEST( tdk__event_loop , schedule ) {
	tdk::event_loop loop;
	int called = 0;
	tdk::timer_task tt( &timer_called , &called );
	tt.expired_at( tdk::date_time::utc() + tdk::time_span::from_milli_seconds(200));
	loop.schedule(&tt);
	loop.run();
	ASSERT_EQ( called , 1);
}

TEST( tdk__event_loop , thread_execute ) {
	tdk::event_loop loop;
	int called = 0;
	int called2= 0;
	tdk::timer_task tt( &timer_called , &called );
	tt.expired_at( tdk::date_time::utc() + tdk::time_span::from_seconds(1));
	loop.schedule(&tt);
	tdk::task thread_execute_task( &thread_execute_called , &called2 );
	std::thread t(
			[&](void){
				loop.execute( &thread_execute_task );
	});

	t.join();
	loop.run();
	ASSERT_EQ( called , 1 );
	ASSERT_EQ( called2 , 1 );
}
