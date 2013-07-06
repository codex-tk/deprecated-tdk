#ifndef __tdk_util_initializer_h__
#define __tdk_util_initializer_h__

namespace tdk {
namespace util {

class initializer {
public:
	template < typename function >
	initializer( function func ) {
		func();
	} 
};

}}


#endif