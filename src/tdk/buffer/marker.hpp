#ifndef __tdk_buffer_marker_h__
#define __tdk_buffer_marker_h__

#include <tdk/tdk.hpp>

namespace tdk { namespace buffer {
	
class marker {
public:
	static const uint32_t INVALID_MARK;
public:
	explicit marker( uint32_t read_mark = INVALID_MARK , uint32_t write_mark = INVALID_MARK ) ;
	marker( const marker& rhs ) ;
	~marker( void );

	uint32_t read_mark( void ) const ;
	uint32_t write_mark( void ) const ;
private:
	marker& operator=( const marker& rhs ) ;
private:
	uint32_t _read_mark;
	uint32_t _write_mark;
};

}}

#endif