#ifndef __tdk_time_date_time_h__
#define __tdk_time_date_time_h__

#include <tdk/time/tick.hpp>

namespace tdk {

//todo timezone 관련하여 수정할 부분 확인하자 
class time_span;
// 1970 01 01 기준
class date_time {
public:
	/*
	static const uint32_t TIME_ZONE_UTC = 0;
    static const uint32_t TIME_ZONE_GMT_9 = 9;

	static const uint64_t TIME_ZONE_MICRO_SECONDS     = TIME_ZONE_GMT_9 * tick::HOUR_TO_MICRO_SECONDS;
	*/
public:
    date_time( void );
	date_time( const uint64_t microSec );
    date_time( const date_time& rhs );
    date_time( const uint16_t year , const uint16_t month ,  const uint16_t day , 
              const uint16_t hour , const uint16_t minute , const uint16_t second );
    date_time& operator=(const date_time& rhs );
    ~date_time( void );
	    
	time::tick::value_type time( void ) const;
	// wrapper 클래스 사용하여 처리 할것
	//TCHAR* to_string( void ) const;
	// formatter fmt( date_time::local());
	// str = fmt.format( "yyyymmddhhnnss" );
	
public:
    bool operator==( const date_time& rhs ) const;
    bool operator!=( const date_time& rhs ) const;
    bool operator> ( const date_time& rhs ) const;
    bool operator>=( const date_time& rhs ) const;
    bool operator< ( const date_time& rhs ) const;
    bool operator<=( const date_time& rhs ) const;

    date_time  operator+ ( const time_span& rhs ) const;
    date_time& operator+=( const time_span& rhs ) ;
    date_time  operator- ( const time_span& rhs ) const;
    date_time& operator-=( const time_span& rhs ) ;
    time_span  operator- ( const date_time& rhs ) const;

public:	
    enum class WEEKDAY {
		SUNDAY	    = 0, 
		MONDAY	    = 1, 
		TUESDAY		= 2, 
		WEDNESDAY	= 3, 
		THURSDAY	= 4, 
		FRIDAY		= 5, 
		SATURDAY	= 6,
    };
	
	int32_t year( void ) const;
	int32_t month( void ) const;
	int32_t day( void ) const; 
	int32_t hour( void ) const; 
	int32_t minute( void ) const;
	int32_t second( void ) const;
	WEEKDAY weekday( void ) const;
	
public: 
	static date_time utc();
	static date_time local();

	/* tick 사용하여 처리 할것
	time_t to_time_t() const;
	struct tm to_tm() const;
    SYSTEMTIME to_systemtime( ) const;
    FILETIME   to_filetime() const;
    static date_time from( const time_t t );
    static date_time from( const SYSTEMTIME& st );
    static date_time from( const FILETIME& ft );
    static date_time from( const uint64_t ms );
	*/
private:
	time::tick::value_type _time;
};

}
#include "time_span.hpp"
#endif