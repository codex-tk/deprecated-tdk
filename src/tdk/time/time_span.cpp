#include "stdafx.h"
#include <cmath>

#include <tdk/time/tick.hpp>
#include <tdk/time/time_span.hpp>

using namespace tdk::time;

namespace tdk {

const int64_t time_span::INFINITE_DELTA = 0x7FFFFFFFFFFFFFFF; //MAXINT64;
const time_span time_span::INFINITE_TIME = time_span( INFINITE_DELTA );

time_span::time_span( void ) : _delta(0) {

}

time_span::~time_span( void ) {

}

time_span::time_span( const time_span& rhs ) : _delta( rhs._delta ) {

}

time_span::time_span( const uint32_t hours 
					 , const uint32_t minutes 
					 , const uint32_t seconds ) 
    : _delta( hours ) {
    _delta *= static_cast<int64_t>(tick::HOUR_TO_MINUTES);
    _delta += minutes;
    _delta *= static_cast<int64_t>(tick::MINUTE_TO_SECONDS);
    _delta += seconds;
    _delta *= static_cast<int64_t>(tick::SECOND_TO_MICRO_SECONDS);
}

time_span::time_span( const uint32_t days 
					 , const uint32_t hours 
					 , const uint32_t minutes 
					 , const uint32_t seconds ) 
: _delta( days ) {
    _delta *= static_cast<const int64_t>(tick::DAY_TO_HOURS);
    _delta += hours;
    _delta *= static_cast<const int64_t>(tick::HOUR_TO_MINUTES);
    _delta += minutes;
    _delta *= static_cast<const int64_t>(tick::MINUTE_TO_SECONDS);
    _delta += seconds;
    _delta *= static_cast<const int64_t>(tick::SECOND_TO_MICRO_SECONDS);    
}

time_span::time_span( const uint32_t days 
					 , const uint32_t hours 
					 , const uint32_t minutes 
					 , const uint32_t seconds 
					 , const uint32_t milliseconds )
: _delta( days ) {
    _delta *= static_cast<int64_t>(tick::DAY_TO_HOURS);
    _delta += hours;
    _delta *= static_cast<int64_t>(tick::HOUR_TO_MINUTES);
    _delta += minutes;
    _delta *= static_cast<int64_t>(tick::MINUTE_TO_SECONDS);
    _delta += seconds;
    _delta *= static_cast<int64_t>(tick::SECOND_TO_MILLI_SECONDS);
    _delta += milliseconds;
    _delta *= static_cast<int64_t>(tick::MILLI_SECOND_TO_MICRO_SECONDS);
}


time_span& time_span::operator=( const time_span& rhs ) {
    _delta = rhs._delta;
    return *this;
}

time_span::time_span( const int64_t microSec ) : _delta(microSec) {

}

int64_t time_span::delta( void ) const {
    return _delta;
}

time_span time_span::from_micro_seconds( const int64_t v ) {
    return time_span( v );
}

time_span time_span::from_milli_seconds( const int64_t v ) {
    return time_span( v * static_cast<int64_t>(tick::MILLI_SECOND_TO_MICRO_SECONDS ));
}

time_span time_span::from_seconds( const int32_t v ) {
    return time_span( v * static_cast<int64_t>(tick::SECOND_TO_MICRO_SECONDS ));
}
time_span time_span::from_minutes( const int32_t v ) {
    return time_span( v * static_cast<int64_t>(tick::MINUTE_TO_MICRO_SECONDS ));
}
time_span time_span::from_hours( const int32_t v ) {
    return time_span( v * static_cast<int64_t>(tick::HOUR_TO_MICRO_SECONDS ));
}
time_span time_span::from_days( const int32_t v ) {
    return time_span( v * static_cast<int64_t>(tick::DAY_TO_MICRO_SECONDS ));
}
time_span time_span::infinite( void ) {
	return time_span( INFINITE_DELTA );
}

timeval time_span::to_timeval( void ) const {
    timeval tval;
	tval.tv_sec  = static_cast<int>(this->total_seconds());
	tval.tv_usec = static_cast<int>(this->total_micro_seconds() % tick::SECOND_TO_MICRO_SECONDS);
    return tval;
}

bool time_span::operator== ( const time_span& rhs ) const{
    return _delta == rhs._delta;
}
bool time_span::operator!=( const time_span& rhs ) const{
    return _delta != rhs._delta;
}
bool time_span::operator> ( const time_span& rhs ) const{
    return _delta > rhs._delta;
}
bool time_span::operator>=( const time_span& rhs ) const{
    return _delta >= rhs._delta;
}
bool time_span::operator< ( const time_span& rhs ) const{
    return _delta < rhs._delta;
}
bool time_span::operator<=( const time_span& rhs ) const{
    return _delta <= rhs._delta;
}

time_span time_span::operator*( const int32_t times ) const{
    return time_span( _delta * times );
}
time_span& time_span::operator*=( const int32_t times ) {
    _delta *= times;
    return *this;
}   

time_span  time_span::operator/ ( const int32_t times ) const{
    return time_span( _delta / (times > 0? times : 1)); //max( 1 , times ) );
}

time_span& time_span::operator/=( const int32_t times ) {
    _delta /= times > 0 ? times : 1;
    return *this;
}

int64_t  time_span::operator/ ( const time_span& times ) const {
    return _delta / ( times._delta > 0 ? times._delta : 1 );
}

time_span  time_span::operator+ ( const time_span& rhs ) const{
    return time_span( _delta + rhs._delta );
}

time_span& time_span::operator+=( const time_span& rhs ) {
    _delta += rhs._delta;
    return *this;
}

time_span  time_span::operator- ( const time_span& rhs ) const{
    return time_span( _delta - rhs._delta );
}

time_span& time_span::operator-=( const time_span& rhs ) {
    _delta -= rhs._delta;
    return *this;
}

date_time  time_span::operator+ ( const date_time& rhs ) const {
    return rhs + *this;
}

time_span  time_span::operator- ( void ) const {
    return time_span( -_delta );
}

int64_t time_span::micro_seconds( void ) const {
	return  total_micro_seconds() % 1000;
}
int64_t time_span::milli_seconds( void ) const {
    return total_milli_seconds() % 1000;
}
int64_t time_span::seconds( void ) const {
    return total_seconds() % 60;
}
int64_t time_span::minutes( void ) const {
    return total_minutes() % 60;
}
int64_t time_span::hours( void ) const {
    return total_hours() % 24;
}
int64_t time_span::days( void ) const {
    return total_days();
}

int64_t time_span::total_micro_seconds( void ) const {
    return _delta;
}
int64_t time_span::total_milli_seconds( void ) const {
    return _delta / static_cast<int64_t>(tick::MILLI_SECOND_TO_MICRO_SECONDS);
}
int64_t time_span::total_seconds( void ) const {
    return _delta / static_cast<int64_t>(tick::SECOND_TO_MICRO_SECONDS);
}
int64_t time_span::total_minutes( void ) const {
    return _delta / static_cast<int64_t>(tick::MINUTE_TO_MICRO_SECONDS);
}
int64_t time_span::total_hours( void ) const {
    return _delta / static_cast<int64_t>(tick::HOUR_TO_MICRO_SECONDS);
}
int64_t time_span::total_days( void ) const {
    return _delta / static_cast<int64_t>(tick::DAY_TO_MICRO_SECONDS);
}


}
