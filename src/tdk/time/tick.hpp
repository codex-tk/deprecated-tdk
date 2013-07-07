#ifndef __tdk_time_tick_h__
#define __tdk_time_tick_h__

#include <tdk/tdk.hpp>

namespace tdk {
namespace time {

// 윈도우 시간은 1601 년 1 월 1 일 부터 100 나노초마다 1 씩 증가
// 유닉스 시간은 1970 년 1 월 1 일 부터   1 초마다    1 씩 증가
// 윈도우 -> 유닉스 시간 변환은 
// 1 초는 1,000,000,000 나노초 
// 윈도우 시간은 100 나노초 이므로  10,000,000 로 나누면 second 단위
// 369 년을 뺌 369 * 365  - 89 (1700 , 1800 , 1900 의 윤년을 제외한 89 일) = 134774
// 134,774 * 24 시간 * 60분 * 60 초 = 11,644,473,600 초 
// 윈도우시간 / 10,000,000 - 11,644,473,600 -> 유닉스 시간
// ( 유닉스시간 + 11,644,473,600 ) * 10,000,000    혹은
// 유닉스시간 * 10,000,000 + 116,444,736,000,000,000 -> 윈도우 시간
                                        
// UTC 기준
class tick {
public:
	static const uint64_t BASE_YEAR						= 1900;
	static const uint64_t EPOCH_YEAR					= 1970;

	static const uint64_t HOUR_TO_MINUTES				= 60;
	static const uint64_t DAY_TO_HOURS					= 24;
	static const uint64_t WEEK_TO_DAYS					= 7;

	static const uint64_t MINUTE_TO_SECONDS				= 60;
	static const uint64_t HOUR_TO_SECONDS				= 60 * 60;
	static const uint64_t DAY_TO_SECONDS				= 60 * 60 * 24;
	static const uint64_t WEEK_TO_SECONDS				= 60 * 60 * 24 * 7;
	static const uint64_t EPOCH_SECONDS					= 11644473600;

	static const uint64_t SECOND_TO_MILLI_SECONDS		= 1000;
	static const uint64_t MILLI_SECOND_TO_MICRO_SECONDS = 1000;

	static const uint64_t SECOND_TO_MICRO_SECONDS	= 1000 * 1000;
	static const uint64_t MINUTE_TO_MICRO_SECONDS	= SECOND_TO_MICRO_SECONDS	* MINUTE_TO_SECONDS;
	static const uint64_t HOUR_TO_MICRO_SECONDS		= SECOND_TO_MICRO_SECONDS	* HOUR_TO_SECONDS;
	static const uint64_t DAY_TO_MICRO_SECONDS		= SECOND_TO_MICRO_SECONDS	* DAY_TO_SECONDS;
	static const uint64_t WEEK_TO_MICRO_SECONDS		= SECOND_TO_MICRO_SECONDS	* WEEK_TO_SECONDS;
	static const uint64_t EPOCH_TO_MICRO_SECONDS	= EPOCH_SECONDS				* SECOND_TO_MICRO_SECONDS;
public:
#if defined(_WIN32) || defined(_WIN64)
	typedef SYSTEMTIME systemtime;
	typedef FILETIME filetime;
#else
	struct systemtime {
		uint16_t wYear;
		uint16_t wMonth;
		uint16_t wDayOfWeek;
		uint16_t wDay;
		uint16_t wHour;
		uint16_t wMinute;
		uint16_t wSecond;
		uint16_t wMilliseconds;
	};
	struct filetime {
		uint32_t dwLowDateTime;
		uint32_t dwHighDateTime;
	};
#endif
public:
	//static uint64_t local( void );
	static uint64_t utc( void );
	static uint64_t from( const tm& t );
	static uint64_t from( const time_t& t );
	static uint64_t from( const timeval& tv );
	static uint64_t from( const systemtime& st );
	static uint64_t from( const filetime& ft );

	static tm			to_tm( const uint64_t v );
	static time_t		to_time_t( const uint64_t v );
	static timeval		to_timeval( const uint64_t v );
	static systemtime	to_systemtime( const uint64_t v );
	static filetime		to_filetime( const uint64_t v );
};


}}

#endif