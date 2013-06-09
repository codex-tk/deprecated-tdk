#include "stdafx.h"
#include <tdk/time/ctime.hpp>
#include <tdk/time/date_time.hpp>

TEST( timezone , t1 ) {
	TIME_ZONE_INFORMATION tz;
	GetTimeZoneInformation( &tz );
	
	time_t t = time(nullptr);

	struct tm tt;
	tdk::time::ctime::localtime( t , tt );

	//tdk::date_time dt( ); 

	tdk::time::tick::systemtime st = tdk::time::tick::to_systemtime( tdk::time::tick::from( tt ));

	return;
	//int bias = tdk::time::ctime::bias();

}