#include "stdafx.h"

#include <tdk/time/ctime.hpp>
#include <tdk/time/tick.hpp>
#include <ctime>

namespace tdk {
namespace time {

uint64_t tick::utc( void ) {
#if defined(_WIN32) || defined(_WIN64)
	filetime ft;
    GetSystemTimeAsFileTime( &ft );
	return tick::from( ft );
#else
    struct timespec ts;
    if( clock_gettime( CLOCK_REALTIME , &ts ) == -1 ) {
        time_t v = std::time(nullptr);
        return tick::from(v);
    }
    return tick::from(ts);
/*  timeval now;
    gettimeofday(&now, NULL);
    return tick::from( now );*/
#endif
}
/*
uint64_t tick::local( void ) {	
#if defined(_WIN32) || defined(_WIN64)
	systemtime st;
	GetLocalTime( &st );
	filetime ft;
    SystemTimeToFileTime(&st, &ft);
	return tick::from( ft );
#else
	timeval now;
    gettimeofday(&now, NULL);
	now.tv_sec -= ( ctime::bias() * 60 );
    return tick::from( now );
#endif
}*/

uint64_t tick::from( const struct tm& t ){
	time_t v = ctime::convert( t );
	return tick::from( v );
}

uint64_t tick::from( const time_t& t ){
	timeval tv;
	tv.tv_sec = static_cast<long>(t);
	tv.tv_usec = 0;
	return tick::from( tv );
}

uint64_t tick::from( const timeval& tv ){
	uint64_t v = tv.tv_sec;
	v *= tick::SECOND_TO_MICRO_SECONDS;
	v += tv.tv_usec;
	return v;
}

uint64_t tick::from( const tick::systemtime& st ){
#if defined(_WIN32) || defined(_WIN64)
	filetime ft;
    SystemTimeToFileTime( &st, &ft );
	return from( ft );
#else
    struct tm tm_date;
    tm_date.tm_hour    = st.wHour;
    tm_date.tm_min     = st.wMinute;
    tm_date.tm_mday    = st.wDay;
    tm_date.tm_mon     = st.wMonth - 1;
    tm_date.tm_sec     = st.wSecond;
    tm_date.tm_year    = st.wYear - 1900;

    time_t t = ctime::convert( tm_date );
    return from(t); ;
#endif
}

#if defined ( _WIN32 ) 

#elif defined( linux ) || defined ( __linux )
uint64_t tick::from( const timespec& ts ){
    uint64_t v = ts.tv_sec;
	v *= tick::SECOND_TO_MICRO_SECONDS;
	v += ts.tv_nsec / 1000;
	return v;
}
#elif defined ( __MACOSX__ ) || defined ( __APPLE__ ) 

#else

#endif


uint64_t tick::from( const tick::filetime& ft ){
	uint64_t v = ft.dwHighDateTime;
	v <<= 32;
	v += ft.dwLowDateTime;
	v /= 10;				// 100 ns 를 microsecond 단위로
	v -= ( tick::EPOCH_SECONDS * tick::SECOND_TO_MICRO_SECONDS );	// 윈도우는 1601 기준이므로 1970 년 사이의 값을 제거 
	return v;
}

struct tm   tick::to_tm( const uint64_t v ){
	time_t t = v / tick::SECOND_TO_MICRO_SECONDS;
	return ctime::convert( t );
}

time_t		tick::to_time_t( const uint64_t v ){
	return v / tick::SECOND_TO_MICRO_SECONDS;
}

timeval		tick::to_timeval( const uint64_t v ){
	timeval tv;
	tv.tv_sec  = static_cast< long >( v / tick::SECOND_TO_MICRO_SECONDS );
	tv.tv_usec = v % tick::SECOND_TO_MICRO_SECONDS;
	return tv;
}

tick::systemtime	tick::to_systemtime( const uint64_t v ){
#if defined(_WIN32) || defined(_WIN64)
	tick::filetime ft = to_filetime( v );
	SYSTEMTIME st;
	FileTimeToSystemTime( (FILETIME*)&ft , &st );
	return tick::systemtime( st );
#else
    systemtime st;
    struct tm tm_date = to_tm( v );
    st.wHour     = tm_date.tm_hour;
    st.wMinute   = tm_date.tm_min ;
    st.wDay      = tm_date.tm_mday;
    st.wMonth    = tm_date.tm_mon + 1;
    st.wSecond   = tm_date.tm_sec ;
    st.wYear     = tm_date.tm_year + 1900;
    st.wMilliseconds = ( v % tick::SECOND_TO_MICRO_SECONDS) / 1000 ;
    return st;
#endif
}

tick::filetime		tick::to_filetime( const uint64_t v ){
	uint64_t tv = v;
	tv += ( tick::EPOCH_SECONDS * tick::SECOND_TO_MICRO_SECONDS );
	tv *= 10;
	filetime ft;
	ft.dwHighDateTime = ( tv & 0xFFFFFFFF00000000 ) >> 32;
	ft.dwLowDateTime  = ( tv & 0xFFFFFFFF		   );
	return ft;
}

}}
