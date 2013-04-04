#ifndef __tdk_time_ctime_h__
#define __tdk_time_ctime_h__

namespace tdk {
namespace time {

class ctime {
public:
	// 윤년
	static bool isleapyear( const int year );
	// 해당 년도에 날짜수 
	static int  daycount( const int year );
	// 해당 년도 사이의 날짜수 
	static int	daycount( const int startYear , const int endYear );
	// 해상 년도의 월일 까지의 날짜수
	static int  daycount( const int year , const int month , const int day );	
	static void gmtime( const time_t aTime , tm& aTm );
	static void localtime( const time_t aTime , tm& aTm );
	static time_t mktime( const tm& aTm );

	// utc = local + bias
	// minute
	// utc - bias = local
	static int  bias();
};

}}

#endif