#include "stdafx.h"

#include <tdk.task/task/timer.hpp>
#include <tdk.task/task/event_loop.hpp>

TEST( timer , t1 ) {
	tdk::task::event_loop& loop = tdk::task::event_loop::default_loop();
	tdk::task::timer timer( loop );

	bool execute = false;
	bool canceled = false;

	timer.expired_at( tdk::date_time::local() + tdk::time_span::from_seconds(1) );
	timer.handler( 
		[&execute,&canceled] ( tdk::error_code& ec ){
			if ( ec ) {
				canceled = true;
			}
			execute = true;
		});

	timer.schedule();
	timer.cancel();

	loop.run();

	ASSERT_TRUE( execute );
	ASSERT_TRUE( canceled );
}