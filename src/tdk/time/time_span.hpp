#ifndef __tdk_time_time_span_h__
#define __tdk_time_time_span_h__

#include <tdk/time/date_time.hpp>

namespace tdk {
	
class time_span {
public:
    static const time_span INFINITE_TIME;
public:
    time_span( void );
    time_span( const time_span& rhs );    
    time_span( const uint32_t hours , const uint32_t minutes , const uint32_t seconds );
    time_span( const uint32_t days  , const uint32_t hours   , const uint32_t minutes , 
              const uint32_t seconds );
    time_span( const uint32_t days  , const uint32_t hours   , const uint32_t minutes , 
              const uint32_t seconds , const uint32_t milliseconds );

	time_span& operator=( const time_span& rhs );    

	~time_span( void );
    int64_t delta( void ) const;
public:
    int64_t micro_seconds( void ) const ;
    int64_t milli_seconds( void ) const ;
    int64_t seconds( void ) const ;
    int64_t minutes( void ) const ;
    int64_t hours( void ) const ;
    int64_t days( void ) const ;

    int64_t total_micro_seconds( void ) const ;
    int64_t total_milli_seconds( void ) const ;
    int64_t total_seconds( void ) const ;
    int64_t total_minutes( void ) const ;
    int64_t total_hours( void ) const ;
    int64_t total_days( void ) const ;

    timeval to_timeval( void ) const;

public:
    bool operator==( const time_span& rhs ) const;
    bool operator!=( const time_span& rhs ) const;
    bool operator> ( const time_span& rhs ) const;
    bool operator>=( const time_span& rhs ) const;
    bool operator< ( const time_span& rhs ) const;
    bool operator<=( const time_span& rhs ) const;

public:
    time_span  operator* ( const int32_t times ) const;     
    time_span& operator*=( const int32_t times );     

    time_span  operator/ ( const int32_t times ) const;     
    time_span& operator/=( const int32_t times );     

    int64_t operator/ ( const time_span& times ) const;     
    
    time_span  operator+ ( const time_span& rhs ) const;
    time_span& operator+=( const time_span& rhs );

    time_span  operator- ( const time_span& rhs ) const;
    time_span& operator-=( const time_span& rhs );

    date_time  operator+ ( const date_time& rhs ) const;
    time_span  operator- ( void ) const;

public:
    static time_span from_micro_seconds( const int64_t v );
    static time_span from_milli_seconds( const int32_t v );
    static time_span from_seconds( const int32_t v );
    static time_span from_minutes( const int32_t v );
    static time_span from_hours( const int32_t v );
    static time_span from_days( const int32_t v );
	static const int64_t INFINITE_DELTA;
	static time_span infinite( void );
private:
    time_span( const int64_t microSec );
private:
    int64_t _delta;
};
}

#endif
