#ifndef __tdk_buffer_marker_h__
#define __tdk_buffer_marker_h__

#include <tdk/tdk.hpp>

namespace tdk { namespace buffer {
	
class marker {
public:
	static const uint32_t INVALID_MARK;
public:
	explicit marker( std::size_t read_mark = INVALID_MARK , std::size_t write_mark = INVALID_MARK ) ;
	marker( const marker& rhs ) ;
	~marker( void );

	std::size_t read_mark( void ) const ;
	std::size_t write_mark( void ) const ;
private:
	marker& operator=( const marker& rhs ) ;
private:
	std::size_t _read_mark;
	std::size_t _write_mark;
};

}}

#endif