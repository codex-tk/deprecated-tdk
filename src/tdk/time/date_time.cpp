#include "stdafx.h"
#include <tdk/time/date_time.hpp>
#include <tdk/time/time_span.hpp>
#include <tdk/time/ctime.hpp>

using namespace tdk::time;

namespace tdk {
	
date_time::date_time( void ) : _time(0) {}

date_time::date_time( const uint64_t microSec ) 
:_time( microSec ) {
}

date_time::date_time( const date_time& rhs ) 
:_time( rhs._time ) {
}

date_time::date_time( const uint16_t year 
					 , const uint16_t month 
					 , const uint16_t day 
					 , const uint16_t hour 
					 , const uint16_t minute 
					 , const uint16_t second  ) 
{
	time_span span = time_span::from_seconds( second );
	span += time_span::from_minutes( minute );
	span += time_span::from_hours( hour );
	span += time_span::from_days( ctime::daycount( 1970 , year -1 ) );
	// day 는 1 ~ .. 이므로 단위로 처리하기 위해선 - 1 해야함/
	span += time_span::from_days( ctime::daycount( year , month , day - 1 ) );
	_time = span.delta();
}

date_time& date_time::operator=(const date_time& rhs ) {
    this->_time = rhs._time;
    return *this;
}

date_time::~date_time( void ) {}

date_time date_time::utc() {
	return date_time( tick::utc() ); 
}

date_time date_time::local() {
	return date_time( tick::local() ); 
}


uint64_t date_time::time( void ) const {
    return _time;
}

bool date_time::operator==( const date_time& rhs ) const{
    return this->_time == rhs._time;
}

bool date_time::operator!=( const date_time& rhs ) const{
    return this->_time != rhs._time;
}

bool date_time::operator> ( const date_time& rhs ) const{
    return this->_time >  rhs._time;
}

bool date_time::operator>=( const date_time& rhs ) const{
    return this->_time >= rhs._time;
}

bool date_time::operator< ( const date_time& rhs ) const{
    return this->_time <  rhs._time;
}

bool date_time::operator<=( const date_time& rhs ) const{
    return this->_time <= rhs._time;
}


date_time  date_time::operator+ ( const time_span& rhs ) const{
    return date_time( _time + rhs.delta());
}

date_time& date_time::operator+=( const time_span& rhs ) {
    _time += rhs.delta();
    return *this;
}

date_time  date_time::operator- ( const time_span& rhs ) const{
    return date_time( _time - rhs.delta());
}

date_time& date_time::operator-=( const time_span& rhs ) {
    _time -= rhs.delta();
    return *this;
}

time_span  date_time::operator- ( const date_time& rhs ) const {
	return time_span::from_micro_seconds( _time - rhs._time );
}

int32_t date_time::year( void ) const {
	return tick::to_tm( _time ).tm_year + 1900;
}

int32_t date_time::month( void ) const{
	return tick::to_tm( _time ).tm_mon + 1;
}

int32_t date_time::day( void ) const{
	return tick::to_tm( _time ).tm_mday;
}

int32_t date_time::hour( void ) const{
	return tick::to_tm( _time ).tm_hour;
}

int32_t date_time::minute( void )const{
	return tick::to_tm( _time ).tm_min;
}

int32_t date_time::second( void )const{
	return tick::to_tm( _time ).tm_sec;
}

date_time::WEEKDAY date_time::weekday( void ) const{
	return static_cast< date_time::WEEKDAY >( tick::to_tm( _time ).tm_wday );
}

}