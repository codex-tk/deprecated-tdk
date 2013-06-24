#ifndef __tdk_buffer_array_h__
#define __tdk_buffer_array_h__

namespace tdk {
namespace buffer {

template < typename T_type >
class array {
public:
	array( T_type* p 
		, int x 
		, int y 
		, int stride );

	~array( void );
private:
	T_type* _buffer;
};

}}

#endif