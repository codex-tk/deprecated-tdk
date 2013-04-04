#include "stdafx.h"

#include <tdk/time/ctime.hpp>
#include <tdk/time/tick.hpp>

namespace tdk {
namespace time {
	
tick::value_type tick::utc( void ) {
#if defined(_WIN32) || defined(_WIN64)
	FILETIME ft;
    GetSystemTimeAsFileTime( &ft );
	tick::filetime paramFt( ft );
	return tick::from( paramFt );
#else
    timeval now;
    gettimeofday(&now, NULL);
    return tick::from( now );
#endif
}

tick::value_type tick::local( void ) {	
	SYSTEMTIME st;
	GetLocalTime( &st );
	FILETIME ft;
    SystemTimeToFileTime(&st, &ft);
	tick::filetime paramFt( ft );
	return tick::from( paramFt );
}

tick::value_type tick::from( const struct tm& t ){
	time_t v = ctime::mktime( t );
	return tick::from( v );
}

tick::value_type tick::from( const time_t& t ){
	timeval tv;
	tv.tv_sec = static_cast<long>(t);
	tv.tv_usec = 0;
	return tick::from( tv );
}

tick::value_type tick::from( const timeval& tv ){
	tick::value_type v = tv.tv_sec;
	v *= tick::SECOND_TO_MICRO_SECONDS;
	v += tv.tv_usec;
	return v;
}

tick::value_type tick::from( const tick::systemtime& st ){
#if defined(_WIN32) || defined(_WIN64)
	filetime ft;
    SystemTimeToFileTime( 
		reinterpret_cast< SYSTEMTIME* >( 
			const_cast< systemtime* >( &st )) , 
		reinterpret_cast< FILETIME* > (&ft) );
	return from( ft );
#else
    struct tm tm_date;
    tm_date.tm_hour    = st.hour;
    tm_date.tm_min     = st.minute;
    tm_date.tm_mday    = st.day;
    tm_date.tm_mon     = st.month - 1;
    tm_date.tm_sec     = st.second;
    tm_date.tm_year    = st.year - 1900;

    time_t t = ctime::mktime( tm_date );
    return from(t); ;
#endif
}

tick::value_type tick::from( const tick::filetime& ft ){
	tick::value_type v;
	v = ft.highDateTime;
	v <<= 32;
	v += ft.lowDateTime;
	v /= 10;				// 100 ns 를 microsecond 단위로
	v -= ( tick::EPOCH_SECONDS * tick::SECOND_TO_MICRO_SECONDS );	// 윈도우는 1601 기준이므로 1970 년 사이의 값을 제거 
	return v;
}

struct tm   tick::to_tm( const tick::value_type v ){
	time_t t = v / tick::SECOND_TO_MICRO_SECONDS;
	struct tm ret;
	ctime::gmtime( t , ret );
	return ret;
}

time_t		tick::to_time_t( const tick::value_type v ){
	return v / tick::SECOND_TO_MICRO_SECONDS;
}

timeval		tick::to_timeval( const tick::value_type v ){
	timeval tv;
	tv.tv_sec  = static_cast< long >( v / tick::SECOND_TO_MICRO_SECONDS );
	tv.tv_usec = v % tick::SECOND_TO_MICRO_SECONDS;
	return tv;
}

tick::systemtime	tick::to_systemtime( const tick::value_type v ){
#if defined(_WIN32) || defined(_WIN64)
	tick::filetime ft = to_filetime( v );
	SYSTEMTIME st;
	FileTimeToSystemTime( (FILETIME*)&ft , &st );
	return tick::systemtime( st );
#else
    systemtime st;
    struct tm tm_date = to_tm( v );
    st.hour     = tm_date.tm_hour;
    st.minute   = tm_date.tm_min ;
    st.day      = tm_date.tm_mday;
    st.month    = tm_date.tm_mon + 1;
    st.second   = tm_date.tm_sec ;
    st.year     = tm_date.tm_year + 1900;
    return st;
#endif
}

tick::filetime		tick::to_filetime( const tick::value_type v ){
	tick::value_type tickV = v;
	tickV += ( tick::EPOCH_SECONDS * tick::SECOND_TO_MICRO_SECONDS );
	tickV *= 10;
	filetime ft;
	ft.highDateTime = ( tickV & 0xFFFFFFFF00000000 ) >> 32;
	ft.lowDateTime  = ( tickV & 0xFFFFFFFF		   );
	return ft;
}

}}
