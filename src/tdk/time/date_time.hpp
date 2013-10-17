#ifndef __tdk_time_date_time_h__
#define __tdk_time_date_time_h__

#include <tdk/time/tick.hpp>

namespace tdk {

//todo timezone ����Ͽ� ������ �κ� Ȯ������ 
class time_span;
// 1970 01 01 ����
class date_time {
public:
    date_time( void );
	date_time( const date_time& rhs );
	explicit date_time( const uint64_t microSec );
    date_time( const uint16_t year 
		, const uint16_t month 
		, const uint16_t day 
		, const uint16_t hour
		, const uint16_t minute 
		, const uint16_t second );

    date_time& operator=(const date_time& rhs );
    ~date_time( void );
	    
	uint64_t time( void ) const;
	uint64_t time_to_milli_seconds( void ) const;
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
    enum class day_of_the_week {
		sunday	    = 0, 
		monday	    = 1, 
		tuesday		= 2, 
		wednesday	= 3, 
		thursday	= 4, 
		friday		= 5, 
		saturday	= 6,
    };	
	int32_t year( void ) const;
	int32_t month( void ) const;
	int32_t day( void ) const; 
	int32_t hour( void ) const; 
	int32_t minute( void ) const;
	int32_t second( void ) const;
	day_of_the_week wday( void ) const;	
public: 
	static date_time utc();
	static date_time local();
	static date_time utc(const date_time& local);
	static date_time local(const date_time& utc);

	static date_time remote( const std::string& name );
	static void remote( const std::string& name , const time_span& diff );

	std::string to_string(void);

private:
	uint64_t _time;
};

}
#include "time_span.hpp"
#endif
